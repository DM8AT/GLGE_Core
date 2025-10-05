/**
 * @file Compression.cpp
 * @author DM8AT
 * @brief implement the compression system
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the compression api
#include "Compression.h"

//include zlib
#include "../../external/zlib/zlib.h"
//add math for ceil
#include <cmath>
//for minimum and maximum
#include <algorithm>
//for memory operations
#include <cstring>

//define how big a single chunk is
#define CHUNK 16384

void glge_Compress(String* input, void* level)
{
    //store a temporary string for the data
    String output = "";
    //convert the void* to the actual compression value
    int compLevel = *((uint32_t*)level);

    //check if the compression level is valid
    if ((compLevel > Z_BEST_COMPRESSION) || (compLevel < Z_DEFAULT_COMPRESSION)) {
        printf("Invalid compression level of %d. Valid compression levels must be in range %d to %d\n", compLevel, Z_BEST_COMPRESSION, Z_DEFAULT_COMPRESSION);
        *input = "";
        return;
    }

    //store two chunks to exchange data
    uchar in[CHUNK];
    uchar out[CHUNK];

    //create the zlib stream
    z_stream strm = { 0 };
    //initialize the stream
    if (deflateInit(&strm, compLevel) != Z_OK) {
        //stop early
        printf("Failed to initialize the compression stream\n");
        *input = "";
        return;
    }

    //store the amount of copied bytes
    uint64_t copied = 0;
    //store if there is still data left
    int flush;
    //store the deflate return value
    int ret = 0;
    //compress the whole string
    do {
        //calculate how much to copy
        uint64_t size = std::min<uint64_t>(CHUNK, input->size() - copied);
        //copy the next chunk of data over
        memcpy(in, input->data() + copied, size);
        //increase the amount of read bytes
        copied += size;

        //check for the end
        flush = (copied == input->size()) ? Z_FINISH : Z_NO_FLUSH;

        //map the next chunk
        strm.next_in = in;
        strm.avail_in = size;

        //compress the whole input buffer
        do {
            //write how much data exists
            strm.avail_out = CHUNK;
            //store where to write the data
            strm.next_out = out;
            ret = deflate(&strm, flush);
            if (ret == Z_STREAM_ERROR) {
                //clean up and stop
                printf("Something went wrong while decompressing. ZLib error code: %d\n", ret);
                (void)deflateEnd(&strm);
                *input = output;
                return;
            }
            //get the amount of new data
            uint32_t gotten = CHUNK - strm.avail_out;
            //add the new data to the output
            output.append((const char*)out, gotten);
        } while (strm.avail_out == 0);

    } while (ret != Z_STREAM_END);

    //clean up
    (void)deflateEnd(&strm);
    //store the output
    *input = output;
}

void glge_Decompress(String* input, void*)
{
    //stop if the input is empty
    if (input->size() < 1) {
        *input = "";
        return;
    }
    
    //store the output string
    String output = "";

    //store the input and output chunks
    uchar in[CHUNK];
    uchar out[CHUNK];

    //store the stream used for decompression
    z_stream strm = { 0 };
    //initialize the stream for decompression
    if (inflateInit(&strm) != Z_OK) {
        //something went wrong
        printf("Failed to initialize the decompression stream\n");
        *input = "";
        return;
    }

    //store how many bytes were copied
    uint64_t copied = 0;
    //store the return value of the inflate function
    int ret = 0;

    //decompress the whole string
    do {
        //copy the data into the input chunk
        uint64_t size = std::min<uint64_t>(CHUNK, input->size() - copied);
        memcpy(in, input->data() + copied, size);
        copied += size;

        //map the next input chunk
        strm.next_in = in;
        strm.avail_in = size;

        //decompress the chunk
        do {
            //store the output chunk
            strm.avail_out = CHUNK;
            strm.next_out = out;

            //decompress the data
            ret = inflate(&strm, Z_NO_FLUSH);
            //error checking
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                //something went wrong - don't forget to clean up
                printf("Something went wrong while decompressing the input. ZLib error code: %d\n", ret);
                inflateEnd(&strm);
                *input = "";
                return;
            }

            //write the data to the output
            uint32_t gotten = CHUNK - strm.avail_out;
            output.append((const char*)out, gotten);

        } while (strm.avail_out == 0);

    } while (ret != Z_STREAM_END);

    //clean up and return
    inflateEnd(&strm);
    *input = output;
}
