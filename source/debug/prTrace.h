/**
 * @file       prTrace.h
 * @brief      Contains tracing functions. The functions come in two flavours. <br>
 *             Trace functions which are removed in release builds. <br>
 *             Log functions which remain in release builds.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRTRACE_H
#define __PRTRACE_H


#if defined(_DEBUG) || defined(DEBUG)

/// @brief   Outputs a debug string to an error log window.
/// @brief   The actual location is platform dependant, but will generally be the debuggers output window.
/// @note    The release version of this function compiles to nothing.
/// @param   fmt - The format string.
/// @param   ... - Format string parameters.
/// <br>
void prTrace(const char* fmt, ...);

/// @brief  Allows tracing to be enabled/disabled. By default tracing is enabled
/// @param  state - true or false.
/// @note   The release version of this function compiles to nothing.
/// <br>
void prTraceEnable(bool state);

/// @brief  Enables or disables duplicate string output. This prevents logging from spamming the debug output window.
/// @param  state - true or false.
/// @note   The release version of this function compiles to nothing.
/// <br>
void prTraceRemoveDuplicates(bool state);

/// @brief  Deletes the old file log.
/// @brief  This function is called by the engine on startup. You do not need to call.
/// @note   The release version of this function compiles to nothing.
/// <br>
void prTraceLogClear();

#else

// Release versions compile to nothing
#define prTrace(fmt, ...)
#define prTraceEnable(state)
#define prTraceRemoveDuplicates(state)
#define prTraceLogClear()

#endif


/// @brief   Outputs a debug string to an error log window.
/// @brief   The actual location is platform dependant, but will generally be the debuggers output window.
/// @param   fmt - The format string.
/// @param   ... - Format string parameters.
/// <br>
void prLog(const char* fmt, ...);


#endif//__PRTRACE_H
