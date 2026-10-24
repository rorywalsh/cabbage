/*
 * Tests for JSONC (JSON with comments) support in the Cabbage backend.
 *
 * Three separate parsing paths are covered:
 *  - widget descriptors read from JS files (File::extractPropsFromJS)
 *  - the <Cabbage> JSON section read from a CSD file (File::parseCabbageSection)
 *  - widget routing for CSD loading (Parser::parseCsdForWidgets)
 */

#include <catch2/catch_test_macros.hpp>
#include "../src/CabbageParser.h"
#include "../src/CabbageUtils.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

// Defined in CabbageAppTests.cpp. Bootstraps the shared settings file when the
// test environment does not have one yet, so widget descriptors can be loaded.
void ensureValidSettingsFileExists();

namespace
{
std::string writeTempCsd(const std::string &contents)
{
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto path = std::filesystem::temp_directory_path() /
                      ("cabbage_jsonc_test_" + std::to_string(stamp) + ".csd");

    std::ofstream file(path, std::ios::binary);
    file << contents;
    file.close();

    return path.string();
}
} // namespace

TEST_CASE("extractPropsFromJS accepts comments inside the props object", "[JSONC]")
{
    const std::string js = R"JS(
// Comments outside the props object are irrelevant.
export class CommentedWidget {
    constructor() {
        this.props = {
            // A line comment before a property.
            "type": "customWidget",
            "bounds": { "top": 0, "left": 0, "width": 100, "height": 30 },
            "channels": [{
                "id": "customWidget",
                /* A block comment between properties. */
                "event": "valueChanged",
                "range": { "min": 0, "max": 1, "value": 0 }
            }]
        };
    }
}
)JS";

    const auto j = cabbage::File::extractPropsFromJS(js);

    REQUIRE(j.is_object());
    REQUIRE(j["type"] == "customWidget");
    REQUIRE(j["channels"].is_array());
    REQUIRE(j["channels"][0]["id"] == "customWidget");
    REQUIRE(j["channels"][0]["range"]["max"] == 1);
}

TEST_CASE("extractPropsFromJS ignores braces in comments and strings", "[JSONC]")
{
    const std::string js = R"JS(
export class BracesWidget {
    constructor() {
        this.props = {
            "type": "customWidget",
            // This comment has a closing brace: } and an opening brace: {
            "label": { "text": "a } brace inside a string" },
            /* Block comment with both: { } */
            "style": { "opacity": 1 }
        };
    }
}
)JS";

    const auto j = cabbage::File::extractPropsFromJS(js);

    REQUIRE(j.is_object());
    REQUIRE(j["type"] == "customWidget");
    REQUIRE(j["label"]["text"] == "a } brace inside a string");
    REQUIRE(j["style"]["opacity"] == 1);
}

TEST_CASE("extractPropsFromJS still rejects malformed props", "[JSONC]")
{
    const std::string js = R"JS(
export class BrokenWidget {
    constructor() {
        this.props = {
            "type": "customWidget",
            "channels": [
        };
    }
}
)JS";

    const auto j = cabbage::File::extractPropsFromJS(js);
    REQUIRE(j.is_null());
}

TEST_CASE("parseCabbageSection accepts comments in the CSD JSON", "[JSONC]")
{
    const std::string csd = R"CSD(<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
endin
</CsInstruments>
<CsScore>
</CsScore>

<Cabbage>
{
    // Top-level comment
    "widgets": [
        {
            "type": "form",
            /* The form defines the editor size. */
            "size": { "width": 640, "height": 360 }
        }
    ],
    "channelConfig": [
        // Named configuration with a block comment.
        { "name": "Stereo", "ins": "2", "outs": "2" }
    ]
}
</Cabbage>
</CsoundSynthesizer>
)CSD";

    const auto path = writeTempCsd(csd);
    const auto parsed = cabbage::File::parseCabbageSection(path);

    std::filesystem::remove(path);

    REQUIRE(parsed.has_value());
    REQUIRE(parsed->contains("widgets"));
    REQUIRE((*parsed)["widgets"].is_array());
    REQUIRE((*parsed)["widgets"][0]["type"] == "form");
    REQUIRE((*parsed)["widgets"][0]["size"]["width"] == 640);
    REQUIRE((*parsed)["channelConfig"][0]["name"] == "Stereo");
}

TEST_CASE("parseCsdForWidgets loads inline JSONC with a commented form", "[JSONC]")
{
    ensureValidSettingsFileExists();

    const std::string csd = R"CSD(<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
endin
</CsInstruments>
<CsScore>
</CsScore>

<Cabbage>
{
    "widgets": [
        {
            "type" /* the form is the editor window */ : /* keep it inline */ "form",
            "size": { "width": 640, "height": 360 }
        },
        {
            "type": "rotarySlider",
            "channels": [
                { "id": "gain", "range": { "min": 0, "max": 1, "value": 0 } }
            ]
        }
    ]
}
</Cabbage>
</CsoundSynthesizer>
)CSD";

    const auto path = writeTempCsd(csd);
    std::string error;
    const auto widgets = cabbage::Parser::parseCsdForWidgets(path, &error);

    std::filesystem::remove(path);

    INFO("parse error: " << error);
    REQUIRE(error.empty());
    REQUIRE(widgets.size() == 2);
    REQUIRE(widgets[0]["type"] == "form");
    REQUIRE(widgets[0]["size"]["width"] == 640);
    REQUIRE(widgets[1]["type"] == "rotarySlider");
    REQUIRE(widgets[1]["channels"][0]["id"] == "gain");
}

TEST_CASE("parseCsdForWidgets still follows an external include", "[JSONC]")
{
    ensureValidSettingsFileExists();

    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto dir = std::filesystem::temp_directory_path() /
                     ("cabbage_jsonc_include_" + std::to_string(stamp));
    std::filesystem::create_directories(dir);

    const auto includePath = dir / "widgets.json";
    {
        std::ofstream includeFile(includePath, std::ios::binary);
        includeFile << R"JSON([
    { "type": "form", "size": { "width": 320, "height": 200 } }
])JSON";
    }

    const std::string csd = R"CSD(<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
endin
</CsInstruments>
<CsScore>
</CsScore>

<Cabbage>
// This section is not inline JSON: the widgets live in the included file.
#include "widgets.json"
</Cabbage>
</CsoundSynthesizer>
)CSD";

    const auto csdPath = dir / "include_test.csd";
    {
        std::ofstream csdFile(csdPath, std::ios::binary);
        csdFile << csd;
    }

    std::string error;
    const auto widgets = cabbage::Parser::parseCsdForWidgets(csdPath.string(), &error);

    std::filesystem::remove_all(dir);

    INFO("parse error: " << error);
    REQUIRE(error.empty());
    REQUIRE(widgets.size() == 1);
    REQUIRE(widgets[0]["type"] == "form");
    REQUIRE(widgets[0]["size"]["width"] == 320);
}

#ifdef TEST_VSCABBAGE_DIR
TEST_CASE("the shipped CustomWidgetTemplate descriptor parses", "[JSONC]")
{
    const std::string path = std::string(TEST_VSCABBAGE_DIR) + "/cabbage/widgets/CustomWidgetTemplate.js";
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        SUCCEED("CustomWidgetTemplate.js is not available in this checkout");
        return;
    }

    const std::string js((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const auto j = cabbage::File::extractPropsFromJS(js);

    REQUIRE(j.is_object());
    REQUIRE(j["type"] == "customWidget");
    REQUIRE(j["channels"].is_array());
}
#endif
