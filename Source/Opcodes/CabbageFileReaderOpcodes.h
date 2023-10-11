#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"

#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/vorbisenc.h"
#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/codec.h"
#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/vorbisfile.h"


struct CabbageFileReader : csnd::Plugin<2, 4>
{
    AudioBuffer<float> audioBuffer;
    File audioFile = {};
    int init();
    int aperf();
    int deinit();    float playbackRate = 1;
    int loopMode = 0;
    int skipTime = 0;
    float sampleIndex[16] = {0};
    bool loopEnded = false;
    std::unique_ptr<LagrangeInterpolator> resample;
    OggVorbis_File file;
    int numSamples = 0;
    int numChannels = 0;
    static size_t read(void* buffer, size_t elementSize, size_t elementCount, void* dataSource) {
        assert(elementSize == 1);

        std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
        stream.read(static_cast<char*>(buffer), elementCount);
        const std::streamsize bytesRead = stream.gcount();
        stream.clear(); // In case we read past EOF
        return static_cast<size_t>(bytesRead);
    }

    static int seek(void* dataSource, ogg_int64_t offset, int origin) {
        static const std::vector<std::ios_base::seekdir> seekDirections{
            std::ios_base::beg, std::ios_base::cur, std::ios_base::end
        };

        std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
        stream.seekg(offset, seekDirections.at(origin));
        stream.clear(); // In case we seeked to EOF
        return 0;
    }

    static long tell(void* dataSource) {
        std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
        const auto position = stream.tellg();
        assert(position >= 0);
        return static_cast<long>(position);
    }

        
    void resampleBuffer(double ratio, AudioBuffer<float>& buffer, int numChannels);
};
