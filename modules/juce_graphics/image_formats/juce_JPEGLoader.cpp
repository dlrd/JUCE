/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-8-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

#include <csetjmp> // SMODE for jmp_buf

namespace juce
{

JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4365 6240 6326 6386 6385 28182 28183 6387 6011 6001)

namespace jpeglibNamespace
{
#if JUCE_INCLUDE_JPEGLIB_CODE || ! defined (JUCE_INCLUDE_JPEGLIB_CODE)
     JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wconversion",
                                          "-Wdeprecated-register",
                                          "-Wdeprecated-declarations",
                                          "-Wsign-conversion",
                                          "-Wcast-align",
                                          "-Wswitch-enum",
                                          "-Wswitch-default",
                                          "-Wimplicit-fallthrough",
                                          "-Wzero-as-null-pointer-constant",
                                          "-Wshift-negative-value",
                                          "-Wcomma")

    #define JPEG_INTERNALS
    #undef FAR
    #include "jpglib/jpeglib.h"

    #include "jpglib/jcapimin.c"
    #include "jpglib/jcapistd.c"
    #include "jpglib/jccoefct.c"
    #include "jpglib/jccolor.c"
    #undef FIX
    #include "jpglib/jcdctmgr.c"
    #undef CONST_BITS
    #include "jpglib/jchuff.c"
    #undef emit_byte
    #include "jpglib/jcinit.c"
    #include "jpglib/jcmainct.c"
    #include "jpglib/jcmarker.c"
    #include "jpglib/jcmaster.c"
    #include "jpglib/jcomapi.c"
    #include "jpglib/jcparam.c"
    // SMODE TECH libjpeg9 #include "jpglib/jcphuff.c"
    #include "jpglib/jcprepct.c"
    #include "jpglib/jcsample.c"
    #define my_coef_controller my_coef_controller2 // SMODE fix name conflict on several compilation unit
    #define my_coef_ptr my_coef_ptr2 // SMODE fix name conflict on several compilation unit
    #define start_iMCU_row start_iMCU_row2 // SMODE fix name conflict on several compilation unit
    #define start_pass_coef start_pass_coef2  // SMODE fix name conflict on several compilation unit
    #define compress_output compress_output2  // SMODE fix name conflict on several compilation unit
    #include "jpglib/jctrans.c"
    #include "jpglib/jdapistd.c"
    #include "jpglib/jdapimin.c"
    #include "jpglib/jdatasrc.c"
    #define my_coef_controller my_coef_controller3 // SMODE fix name conflict on several compilation unit
    #define my_coef_ptr my_coef_ptr3 // SMODE fix name conflict on several compilation unit
    #include "jpglib/jdcoefct.c"
    #undef FIX
    #define my_color_converter my_color_converter2 // SMODE fix name conflict on several compilation unit
    #define my_cconvert_ptr my_cconvert_ptr2 // SMODE fix name conflict on several compilation unit
    #include "jpglib/jdcolor.c"
    #undef FIX
    #include "jpglib/jddctmgr.c"
    #undef CONST_BITS
    #undef ASSIGN_STATE

    #define savable_state savable_state2 // SMODE fix name conflict on several compilation unit
    #define huff_entropy_encoder huff_entropy_encoder2 // SMODE fix name conflict on several compilation unit
    #define huff_entropy_ptr huff_entropy_ptr2 // SMODE fix name conflict on several compilation unit

    #include "jpglib/jdhuff.c"
    #include "jpglib/jdinput.c"

    #define my_main_controller my_main_controller2 // SMODE fix name conflict on several compilation unit
    #define my_main_ptr my_main_ptr2 // SMODE fix name conflict on several compilation unit
    #include "jpglib/jdmainct.c"

    #define my_marker_ptr  my_marker_writer_ptr // SMODE fix name conflict on several compilation unit
    #define my_master_ptr my_decomp_master_ptr // SMODE fix name conflict on several compilation unit
    #include "jpglib/jdmarker.c"
    #include "jpglib/jdmaster.c"
    #undef FIX
    #define build_ycc_rgb_table build_ycc_rgb_table2 // SMODE fix name conflict on several compilation unit
    #define build_bg_ycc_rgb_table build_bg_ycc_rgb_table2 // SMODE fix name conflict on several compilation unit
    #include "jpglib/jdmerge.c"
    #undef ASSIGN_STATE
    // SMODE TECH libjpeg9 #include "jpglib/jdphuff.c"
    #include "jpglib/jdpostct.c"
    #undef FIX
    #define my_upsampler my_upsampler2 // SMODE fix name conflict on several compilation unit
    #define my_upsample_ptr my_upsample_ptr2 // SMODE fix name conflict on several compilation unit
    #include "jpglib/jdsample.c"
    #include "jpglib/jdtrans.c"
    #include "jpglib/jfdctflt.c"
    #include "jpglib/jfdctint.c"
    #undef CONST_BITS
    #undef MULTIPLY
    #undef FIX_0_541196100
    #include "jpglib/jfdctfst.c"
    #undef FIX_0_541196100
    #include "jpglib/jidctflt.c"
    #undef CONST_BITS
    #undef FIX_1_847759065
    #undef MULTIPLY
    #undef DEQUANTIZE
    #undef DESCALE
    #include "jpglib/jidctfst.c"
    #undef CONST_BITS
    #undef FIX_1_847759065
    #undef MULTIPLY
    #undef DEQUANTIZE
    #include "jpglib/jidctint.c"
    // SMODE TECH libjpeg9 #include "jpglib/jidctred.c"
    #include "jpglib/jmemmgr.c"
    #include "jpglib/jmemnobs.c"
    #include "jpglib/jquant1.c"
    #define my_cquantizer my_cquantizer2 // SMODE fix name conflict on several compilation unit
    #define my_cquantize_ptr my_cquantize_ptr2 // SMODE fix name conflict on several compilation unit
    #include "jpglib/jquant2.c"
    #include "jpglib/jutils.c"
    #include "jpglib/transupp.c"
    
    # include "jpglib/jaricom.c"  // SMODE TECH libjpeg9 
    #define encode_mcu_DC_first encode_mcu_DC_first2 // SMODE fix name conflict on several compilation unit
    #define encode_mcu_AC_first encode_mcu_AC_first2 // SMODE fix name conflict on several compilation unit
    #define encode_mcu_DC_refine encode_mcu_DC_refine2 // SMODE fix name conflict on several compilation unit
    #define encode_mcu_AC_refine encode_mcu_AC_refine2  // SMODE fix name conflict on several compilation unit
    # include "jpglib/jcarith.c"  // SMODE TECH libjpeg9 
    #define arith_entropy_decoder arith_entropy_decoder2 // SMODE fix name conflict on several compilation unit
    #define arith_entropy_ptr arith_entropy_ptr2 // SMODE fix name conflict on several compilation unit
    #define process_restart process_restart2 // SMODE fix name conflict on several compilation unit
    #define decode_mcu_DC_first decode_mcu_DC_first2 // SMODE fix name conflict on several compilation unit
    #define decode_mcu_AC_first decode_mcu_AC_first2 // SMODE fix name conflict on several compilation unit
    #define decode_mcu_DC_refine decode_mcu_DC_refine2 // SMODE fix name conflict on several compilation unit
    #define decode_mcu_AC_refine decode_mcu_AC_refine2 // SMODE fix name conflict on several compilation unit
    # include "jpglib/jdarith.c"  // SMODE TECH libjpeg9 

    JUCE_END_IGNORE_WARNINGS_GCC_LIKE
#else
    #define JPEG_INTERNALS
    #undef FAR
    #include <jpeglib.h>
#endif
}

#undef max
#undef min

JUCE_END_IGNORE_WARNINGS_MSVC

//==============================================================================
namespace JPEGHelpers
{
    using namespace jpeglibNamespace;

    struct my_error_mgr  // SMODE
    {
        struct jpeg_error_mgr pub;
        jmp_buf setjmp_buffer;
    };
    typedef struct my_error_mgr* my_error_ptr;


   #if ! (JUCE_WINDOWS && (JUCE_MSVC || JUCE_CLANG))
    using jpeglibNamespace::boolean;
   #endif

    static void fatalErrorHandler (j_common_ptr p)          
    { 
      *((bool*) (p->client_data)) = true; 
      // SMODE avoid jpeg error to do not return and crash 
      my_error_ptr myerr = (my_error_ptr)p->err;
      longjmp(myerr->setjmp_buffer, 1);
    }
    static void silentErrorCallback1 (j_common_ptr)         {}
    static void silentErrorCallback2 (j_common_ptr, int)    {}
    static void silentErrorCallback3 (j_common_ptr, char*)  {}

    static void setupSilentErrorHandler (struct jpeg_error_mgr& err)
    {
        zerostruct (err);

        err.error_exit      = fatalErrorHandler;
        err.emit_message    = silentErrorCallback2;
        err.output_message  = silentErrorCallback1;
        err.format_message  = silentErrorCallback3;
        err.reset_error_mgr = silentErrorCallback1;
    }

    //==============================================================================
   #if ! JUCE_USING_COREIMAGE_LOADER
    static void dummyCallback1 (j_decompress_ptr) {}

    static void jpegSkip (j_decompress_ptr decompStruct, long num)
    {
        decompStruct->src->next_input_byte += num;

        num = jmin (num, (long) decompStruct->src->bytes_in_buffer);
        decompStruct->src->bytes_in_buffer -= (size_t) num;
    }

    static boolean jpegFill (j_decompress_ptr)
    {
        return 0;
    }
   #endif

    //==============================================================================
    const int jpegBufferSize = 512;

    struct JuceJpegDest final : public jpeg_destination_mgr
    {
        OutputStream* output;
        char* buffer;
    };

    static void jpegWriteInit (j_compress_ptr) {}

    static void jpegWriteTerminate (j_compress_ptr cinfo)
    {
        JuceJpegDest* const dest = static_cast<JuceJpegDest*> (cinfo->dest);

        const size_t numToWrite = jpegBufferSize - dest->free_in_buffer;
        dest->output->write (dest->buffer, numToWrite);
    }

    static boolean jpegWriteFlush (j_compress_ptr cinfo)
    {
        JuceJpegDest* const dest = static_cast<JuceJpegDest*> (cinfo->dest);

        const int numToWrite = jpegBufferSize;

        dest->next_output_byte = reinterpret_cast<JOCTET*> (dest->buffer);
        dest->free_in_buffer = jpegBufferSize;

        return (boolean) dest->output->write (dest->buffer, (size_t) numToWrite);
    }
}

//==============================================================================
JPEGImageFormat::JPEGImageFormat()
    : quality (-1.0f)
{
}

JPEGImageFormat::~JPEGImageFormat() {}

void JPEGImageFormat::setQuality (const float newQuality)
{
    quality = newQuality;
}

String JPEGImageFormat::getFormatName()                   { return "JPEG"; }
bool JPEGImageFormat::usesFileExtension (const File& f)   { return f.hasFileExtension ("jpeg;jpg"); }

bool JPEGImageFormat::canUnderstand (InputStream& in)
{
    const int bytesNeeded = 24;
    uint8 header [bytesNeeded];

    if (in.read (header, bytesNeeded) == bytesNeeded
            && header[0] == 0xff
            && header[1] == 0xd8
            && header[2] == 0xff)
        return true;

   #if JUCE_USING_COREIMAGE_LOADER
    return header[20] == 'j'
        && header[21] == 'p'
        && header[22] == '2'
        && header[23] == ' ';
   #endif

    return false;
}

#if JUCE_USING_COREIMAGE_LOADER
 Image juce_loadWithCoreImage (InputStream& input);
#endif

Image JPEGImageFormat::decodeImage (InputStream& in)
{
   #if JUCE_USING_COREIMAGE_LOADER
    return juce_loadWithCoreImage (in);
   #else
    using namespace jpeglibNamespace;
    using namespace JPEGHelpers;

    MemoryOutputStream mb;
    mb << in;

    Image image;

    if (mb.getDataSize() > 16)
    {
        struct jpeg_decompress_struct jpegDecompStruct;

        struct my_error_mgr jerr; // SMODE
        setupSilentErrorHandler (jerr.pub);
        jpegDecompStruct.err = &jerr.pub;

        if (setjmp(jerr.setjmp_buffer))  // SMODE
        {
            jpeg_destroy_decompress(&jpegDecompStruct);
            return image;
        }

        jpeg_create_decompress (&jpegDecompStruct);

        jpegDecompStruct.src = (jpeg_source_mgr*)(jpegDecompStruct.mem->alloc_small)
            ((j_common_ptr)(&jpegDecompStruct), JPOOL_PERMANENT, sizeof (jpeg_source_mgr));

        bool hasFailed = false;
        jpegDecompStruct.client_data = &hasFailed;

        jpegDecompStruct.src->init_source       = dummyCallback1;
        jpegDecompStruct.src->fill_input_buffer = jpegFill;
        jpegDecompStruct.src->skip_input_data   = jpegSkip;
        jpegDecompStruct.src->resync_to_restart = jpeg_resync_to_restart;
        jpegDecompStruct.src->term_source       = dummyCallback1;

        jpegDecompStruct.src->next_input_byte   = static_cast<const unsigned char*> (mb.getData());
        jpegDecompStruct.src->bytes_in_buffer   = mb.getDataSize();

        jpeg_read_header (&jpegDecompStruct, TRUE);

        if (! hasFailed)
        {
            jpeg_calc_output_dimensions (&jpegDecompStruct);

            if (! hasFailed)
            {
                const int width  = (int) jpegDecompStruct.output_width;
                const int height = (int) jpegDecompStruct.output_height;

                jpegDecompStruct.out_color_space = JCS_RGB;

                JSAMPARRAY buffer
                    = (*jpegDecompStruct.mem->alloc_sarray) ((j_common_ptr) &jpegDecompStruct,
                                                             JPOOL_IMAGE,
                                                             (JDIMENSION) width * 3, 1);

                if (jpeg_start_decompress (&jpegDecompStruct) && ! hasFailed)
                {
                    image = Image (Image::RGB, width, height, false);
                    image.getProperties()->set ("originalImageHadAlpha", false);
                    const bool hasAlphaChan = image.hasAlphaChannel(); // (the native image creator may not give back what we expect)

                    const Image::BitmapData destData (image, Image::BitmapData::writeOnly);

                    for (int y = 0; y < height; ++y)
                    {
                        jpeg_read_scanlines (&jpegDecompStruct, buffer, 1);

                        if (hasFailed)
                            break;

                        const uint8* src = *buffer;
                        uint8* dest = destData.getLinePointer (y);

                        if (hasAlphaChan)
                        {
                            for (int i = width; --i >= 0;)
                            {
                                ((PixelARGB*) dest)->setARGB (0xff, src[0], src[1], src[2]);
                                ((PixelARGB*) dest)->premultiply();
                                dest += destData.pixelStride;
                                src += 3;
                            }
                        }
                        else
                        {
                            for (int i = width; --i >= 0;)
                            {
                                ((PixelRGB*) dest)->setARGB (0xff, src[0], src[1], src[2]);
                                dest += destData.pixelStride;
                                src += 3;
                            }
                        }
                    }

                    if (! hasFailed)
                        jpeg_finish_decompress (&jpegDecompStruct);

                    in.setPosition (((char*) jpegDecompStruct.src->next_input_byte) - (char*) mb.getData());
                }
            }
        }

        jpeg_destroy_decompress (&jpegDecompStruct);
    }

    return image;
   #endif
}

bool JPEGImageFormat::writeImageToStream (const Image& image, OutputStream& out)
{
    using namespace jpeglibNamespace;
    using namespace JPEGHelpers;

    jpeg_compress_struct jpegCompStruct;
    zerostruct (jpegCompStruct);
    jpeg_create_compress (&jpegCompStruct);

    struct my_error_mgr jerr; // SMODE
    setupSilentErrorHandler (jerr.pub);
    jpegCompStruct.err = &jerr.pub;

    if (setjmp(jerr.setjmp_buffer))  // SMODE
    {
      jpeg_destroy_compress(&jpegCompStruct);
      return false;
    }

    JuceJpegDest dest;
    jpegCompStruct.dest = &dest;

    dest.output = &out;
    HeapBlock<char> tempBuffer (jpegBufferSize);
    dest.buffer = tempBuffer;
    dest.next_output_byte = (JOCTET*) dest.buffer;
    dest.free_in_buffer = jpegBufferSize;
    dest.init_destination = jpegWriteInit;
    dest.empty_output_buffer = jpegWriteFlush;
    dest.term_destination = jpegWriteTerminate;

    jpegCompStruct.image_width  = (JDIMENSION) image.getWidth();
    jpegCompStruct.image_height = (JDIMENSION) image.getHeight();
    jpegCompStruct.input_components = 3;
    jpegCompStruct.in_color_space = JCS_RGB;
    jpegCompStruct.write_JFIF_header = 1;

    jpegCompStruct.X_density = 72;
    jpegCompStruct.Y_density = 72;

    jpeg_set_defaults (&jpegCompStruct);

    jpegCompStruct.dct_method = JDCT_FLOAT;
    jpegCompStruct.optimize_coding = 1;

    if (quality < 0.0f)
        quality = 0.85f;

    jpeg_set_quality (&jpegCompStruct, jlimit (0, 100, roundToInt (quality * 100.0f)), TRUE);

    jpeg_start_compress (&jpegCompStruct, TRUE);

    const int strideBytes = (int) (jpegCompStruct.image_width * (unsigned int) jpegCompStruct.input_components);

    JSAMPARRAY buffer = (*jpegCompStruct.mem->alloc_sarray) ((j_common_ptr) &jpegCompStruct,
                                                             JPOOL_IMAGE, (JDIMENSION) strideBytes, 1);

    const Image::BitmapData srcData (image, Image::BitmapData::readOnly);

    while (jpegCompStruct.next_scanline < jpegCompStruct.image_height)
    {
        uint8* dst = *buffer;

        if (srcData.pixelFormat == Image::RGB)
        {
            const uint8* src = srcData.getLinePointer ((int) jpegCompStruct.next_scanline);

            for (int i = srcData.width; --i >= 0;)
            {
                *dst++ = ((const PixelRGB*) src)->getRed();
                *dst++ = ((const PixelRGB*) src)->getGreen();
                *dst++ = ((const PixelRGB*) src)->getBlue();
                src += srcData.pixelStride;
            }
        }
        else
        {
            for (int x = 0; x < srcData.width; ++x)
            {
                const Colour pixel (srcData.getPixelColour (x, (int) jpegCompStruct.next_scanline));
                *dst++ = pixel.getRed();
                *dst++ = pixel.getGreen();
                *dst++ = pixel.getBlue();
            }
        }

        jpeg_write_scanlines (&jpegCompStruct, buffer, 1);
    }

    jpeg_finish_compress (&jpegCompStruct);
    jpeg_destroy_compress (&jpegCompStruct);

    return true;
}

} // namespace juce
