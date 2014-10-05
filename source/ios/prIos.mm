/**
 * prIos.mm
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "../prConfig.h"


#if defined(PLATFORM_IOS)


#import <Foundation/Foundation.h>
#include "prIos.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../core/prStringUtil.h"


/// ---------------------------------------------------------------------------
/// Gets the applications data directory.
/// ---------------------------------------------------------------------------
const char *prGetAppDataPath()
{
	NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
	
	const char *path = [resourcePath UTF8String];

	return path;
}


/// ---------------------------------------------------------------------------
/// Gets the app documents path.
/// ---------------------------------------------------------------------------
void prGetDocumentsPath(char *buffer)
{
	if (buffer)
	{
		char path_doc[256];

		// Get the documents path
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = paths[0];
	
		// Create documents path
		const char *appPath = [documentsDirectory UTF8String];
		strcpy(path_doc, appPath);
        strcat(path_doc, "/");

        // Copy
		strcpy(buffer, (const char *)path_doc);
        //prTrace("Documents paths: %s\n", buffer);
	}
}


/// ---------------------------------------------------------------------------
/// Loads an audio file.
/// ---------------------------------------------------------------------------
void *prOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei *outSampleRate)
{
    PRASSERT(outDataSize);
    PRASSERT(outDataFormat);
    PRASSERT(outSampleRate);
    
	OSStatus						err                     = noErr;	
	SInt64							theFileLengthInFrames   = 0;
	AudioStreamBasicDescription		theFileFormat;
	UInt32							thePropertySize         = sizeof(theFileFormat);
	ExtAudioFileRef					extRef                  = NULL;
	void*							theData                 = NULL;
	AudioStreamBasicDescription		theOutputFormat;
    
    
	// Open a file with ExtAudioFileOpen()
	err = ExtAudioFileOpenURL(inFileURL, &extRef);
	if (err) 
    { 
        prTrace("OpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %ld\n", err);
        return theData;
    }
	
    
	// Get the audio data format
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
	if (err) 
    {
        prTrace("OpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", err);
        return theData;
    }
    
	if (theFileFormat.mChannelsPerFrame > 2)
    { 
        prTrace("OpenALAudioData - Unsupported Format, channel count is greater than stereo\n");
        return theData;
    }
    
	// Set the client format to 16 bit signed integer (native-endian) data
	// Maintain the channel count and sample rate of the original source format
	theOutputFormat.mSampleRate         = theFileFormat.mSampleRate;
	theOutputFormat.mChannelsPerFrame   = theFileFormat.mChannelsPerFrame;    
	theOutputFormat.mFormatID           = kAudioFormatLinearPCM;
	theOutputFormat.mBytesPerPacket     = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mFramesPerPacket    = 1;
	theOutputFormat.mBytesPerFrame      = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mBitsPerChannel     = 16;
	theOutputFormat.mFormatFlags        = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	
	// Set the desired client (output) data format
	err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
	if (err)
    {
        prTrace("OpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ld\n", err);
        return theData;
    }
	
	// Get the total frame count
	thePropertySize = sizeof(theFileLengthInFrames);
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
	if (err)
    {
        prTrace("OpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ld\n", err);
        return theData;
    }
	
	// Read all the data into memory
	UInt32 dataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;;
	theData = malloc(dataSize);
	if (theData)
	{
		AudioBufferList theDataBuffer;
		theDataBuffer.mNumberBuffers = 1;
		theDataBuffer.mBuffers[0].mDataByteSize   = dataSize;
		theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
		theDataBuffer.mBuffers[0].mData           = theData;
		
		// Read the data into an AudioBufferList
		err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
		if (err == noErr)
		{
			// success
			*outDataSize   = (ALsizei)dataSize;
			*outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			*outSampleRate = (ALsizei)theOutputFormat.mSampleRate;
		}
		else 
		{ 
			// failure
			free (theData);
			theData = NULL;
			prTrace("OpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err);
		}	
	}
	
Exit:
	// Dispose the ExtAudioFileRef, it is no longer needed
	if (extRef)
    {
        ExtAudioFileDispose(extRef);
    }
    
	return theData;
}


/// ---------------------------------------------------------------------------
/// Loads an audio file.
/// ---------------------------------------------------------------------------
void *prLoadAudioFile(const char *filename, ALsizei *size, ALsizei *freq, ALenum *format)
{
    PRASSERT(size);
    PRASSERT(freq);
    PRASSERT(format);
    
    void *pAudioData = NULL;
    
    if (filename && *filename)
    {
        NSBundle *bundle    = [NSBundle mainBundle];
    
        NSString *musicFile = [[NSString alloc] initWithUTF8String:filename];

        // Get the audio data from the file which has been passed in
        NSString *musicFileName = [[musicFile lastPathComponent] stringByDeletingPathExtension];
        NSString *musicFileType =  [musicFile pathExtension];
        
        // Ensure lowercase for our file system.
        musicFileName = [musicFileName lowercaseString];
        musicFileType = [musicFileType lowercaseString];
        
        CFURLRef  fileURL       = (CFURLRef)[[NSURL fileURLWithPath:[bundle pathForResource:musicFileName ofType:musicFileType]] retain];

        if (fileURL)
        {	
            pAudioData = prOpenALAudioData(fileURL, size, format, freq);
            CFRelease(fileURL);
        }
    }
    
    return pAudioData;
}


#endif
