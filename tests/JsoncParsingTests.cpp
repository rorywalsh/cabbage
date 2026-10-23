/*
 * Tests for JSONC (JSON with comments) support in the Cabbage backend.
 *
 * Two separate parsing paths are covered:
 *  - widget descriptors read from JS files (File::extractPropsFromJS)
 *  - the <Cabbage> JSON section read from a CSD file (File::parseCabbageSection)
 */

#include <catch2/catch_test_macros.hpp>
#include "../src/CabbageUtils.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

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
