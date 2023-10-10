#pragma once


#include "../CabbageCommonHeaders.h"
#include "../Widgets/CabbageWidgetData.h"
#undef _CR
#include <plugin.h>

#include "JuceHeader.h"


//JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4267 4127 4244 4996 4100 4701 4702 4013 4133 4206 4305 4189 4706 4995 4365 4456 4457 4459 6297 6011 6001 6308 6255 6386 6385 6246 6387 6263 6262 28182)
//
//JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wconversion",
//                                     "-Wshadow",
//                                     "-Wfloat-conversion",
//                                     "-Wdeprecated-register",
//                                     "-Wdeprecated-declarations",
//                                     "-Wswitch-enum",
//                                     "-Wzero-as-null-pointer-constant",
//                                     "-Wsign-conversion",
//                                     "-Wswitch-default",
//                                     "-Wredundant-decls",
//                                     "-Wmisleading-indentation",
//                                     "-Wmissing-prototypes",
//                                     "-Wcast-align")


#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/vorbisenc.h"
#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/codec.h"
#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/vorbisfile.h"
//




//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/bitwise.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/framing.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/analysis.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/bitrate.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/block.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/codebook.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/envelope.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/floor0.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/floor1.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/info.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lpc.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lsp.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/mapping0.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/mdct.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/psy.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/registry.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/res0.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/sharedbook.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/smallft.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/synthesis.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/vorbisenc.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/vorbisfile.c"
//#include "../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/window.c"


struct CabbageFileReader : csnd::Plugin<2, 4>
{
    AudioBuffer<float> audioBuffer;
    File audioFile = {};
    int init();
    int aperf();
    int deinit(){
        ov_clear (&file);
        return OK;
    }
    float playbackRate = 1;
    int loopMode = 0;
    int skipTime = 0;
    float sampleIndex[16] = {0};
    bool loopEnded = false;
    std::unique_ptr<LagrangeInterpolator> resample;
    OggVorbis_File file;
    
    static size_t read(void* buffer, size_t elementSize, size_t elementCount, void* dataSource) {
        assert(elementSize == 1);

        std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
        stream.read(static_cast<char*>(buffer), elementCount);
        const std::streamsize bytesRead = stream.gcount();
        stream.clear(); // In case we read past EOF
        return static_cast<size_t>(bytesRead);
    }
    
//    static size_t oggReadCallback (void* ptr, size_t size, size_t nmemb, void* datasource)
//    {
//        return (size_t) (static_cast<InputStream*> (datasource)->read (ptr, (int) (size * nmemb))) / size;
//    }
    
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
