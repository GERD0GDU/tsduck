//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2017, Thierry Lelegard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------
//
//  Abstract interface for event reporting and monitoring
//
//----------------------------------------------------------------------------

#include "tsReport.h"
TSDUCK_SOURCE;

#if defined (TS_NEED_STATIC_CONST_DEFINITIONS)
const int ts::Severity::Fatal;
const int ts::Severity::Severe;
const int ts::Severity::Error;
const int ts::Severity::Warning;
const int ts::Severity::Info;
const int ts::Severity::Verbose;
const int ts::Severity::Debug;
#endif


//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

ts::Report::Report(int max_severity) :
    _max_severity(max_severity)
{
}


//----------------------------------------------------------------------------
// Set maximum debug level.
//----------------------------------------------------------------------------

void ts::Report::setMaxSeverity(int level)
{
    _max_severity = level;
    if (level >= Severity::Debug) {
        log(level, u"debug level set to %d", {level});
    }
}

void ts::Report::raiseMaxSeverity(int level)
{
    if (_max_severity < level) {
        setMaxSeverity(level);
    }
}


//----------------------------------------------------------------------------
// Formatted line prefix header for a severity
//----------------------------------------------------------------------------

ts::UString ts::Severity::Header(int severity)
{
    if (severity < Fatal) {
        // Invalid / undefined severity.
        return UString::Format(u"[%d] ", {severity});
    }
    else if (severity > Debug) {
        return UString::Format(u"Debug[%d]: ", {severity});
    }
    else {
        switch (severity) {
            case Fatal:
                return u"FATAL ERROR: ";
            case Severe:
                return u"SEVERE ERROR: ";
            case Error:
                return u"Error: ";
            case Warning:
                return u"Warning: ";
            case Debug:
                return u"Debug: ";
            default: // Including Info and Verbose
                return UString();
        }
    }
}


//----------------------------------------------------------------------------
// Message logging.
//----------------------------------------------------------------------------

void ts::Report::log(int severity, const UString& msg)
{
    if (severity <= _max_severity) {
        writeLog(severity, msg);
    }
}

void ts::Report::log(int severity, const UChar* fmt, const std::initializer_list<ArgMixIn> args)
{
    if (severity <= _max_severity) {
        log(severity, UString::Format(fmt, args));
    }
}

void ts::Report::log(int severity, const UString& fmt, const std::initializer_list<ArgMixIn> args)
{
    if (severity <= _max_severity) {
        log(severity, UString::Format(fmt, args));
    }
}