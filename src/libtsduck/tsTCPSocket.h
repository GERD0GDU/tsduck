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
//!
//!  @file
//!  TCP Socket
//!
//----------------------------------------------------------------------------

#pragma once
#include "tsSocketAddress.h"
#include "tsAbortInterface.h"
#include "tsReportInterface.h"
#include "tsNullReport.h"
#include "tsSafePtr.h"
#include "tsNullMutex.h"
#include "tsMutex.h"

namespace ts {
    //!
    //! Base class for TCP/IP sockets.
    //!
    //! This base class is not supposed to be directly instantiated.
    //! The two concrete subclasses of TCPSocket are:
    //! - TCPServer: A TCP/IP server socket which listens to incoming connections.
    //!   This type is socket is not designed to exchange data.
    //! - TCPConnection: A TCP/IP session between a client and a server. This
    //!   socket can exchange data.
    //!   - A TCP/IP client creates a TCPConnection instance and @e connects to a server.
    //!   - A TCP/IP server creates a TCPServer instance and @e waits for clients. For each
    //!     client session, a TCPConnection instance is created.
    //!
    class TSDUCKDLL TCPSocket
    {
    public:
        //!
        //! Constructor.
        //!
        TCPSocket() :
            _sock(TS_SOCKET_T_INVALID)
        {
        }

        //!
        //! Destructor.
        //!
        virtual ~TCPSocket()
        {
            close(NULLREP);
        }

        //!
        //! Open the socket.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool open(ReportInterface& report = CERR);

        //!
        //! Close the socket.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool close(ReportInterface& report = CERR);

        //!
        //! Check if socket is open.
        //! @return True if socket is open.
        //!
        bool isOpen() const {return _sock != TS_SOCKET_T_INVALID;}

        //!
        //! Set the "send buffer size".
        //! @param [in] size The buffer size in bytes to set.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool setSendBufferSize(size_t size, ReportInterface& report = CERR);

        //!
        //! Set the "receive buffer size".
        //! @param [in] size The buffer size in bytes to set.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool setReceiveBufferSize(size_t size, ReportInterface& report = CERR);

        //!
        //! Set the "reuse port" option.
        //! @param [in] active If true, the socket is allowed to reuse a local
        //! TCP port which is already bound.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool reusePort(bool active, ReportInterface& report = CERR);

        //!
        //! Set the Time To Live (TTL) option.
        //! @param [in] ttl The TTL value, ie. the maximum number of "hops" between
        //! routers before an IP packet is dropped.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool setTTL(int ttl, ReportInterface& report = CERR);

        //!
        //! Remove the linger time option.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool setNoLinger(ReportInterface& report = CERR);

        //!
        //! Set the linger time option.
        //! @param [in] seconds Number of seconds to wait after shuting down the socket.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool setLingerTime(int seconds, ReportInterface& report = CERR);

        //!
        //! Set the "keep alive" option.
        //! @param [in] active If true, the socket periodically sends "keep alive"
        //! packets when the connection is idle.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool setKeepAlive(bool active, ReportInterface& report = CERR);

        //!
        //! Set the "no delay" option.
        //! @param [in] active If true, the socket immediately sends outgoing packets.
        //! By default, a TCP socket waits a small amount of time after a send()
        //! operation to get a chance to group outgoing data from successive send()
        //! operations into one single packet.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool setNoDelay(bool active, ReportInterface& report = CERR);

        //!
        //! Bind to a local address and port.
        //!
        //! The IP address part of the socket address must one of:
        //! - @link IPAddress::AnyAddress @endlink. Any local interface may be used
        //!   to connect to a server (client side) or to receive incoming client
        //!   connections (server side).
        //! - The IP address of an interface of the local system. Outgoing connections
        //!   (client side) will be only allowed through this interface. Incoming client
        //!   connections (server side) will be accepted only when they arrive through
        //!   the selected interface.
        //! 
        //! The port number part of the socket address must be one of:
        //! - @link SocketAddress::AnyPort @endlink. The socket is bound to an
        //!   arbitrary unused local TCP port. This is the usual configuration for
        //!   a TCP client.
        //! - A specific port number. This is the usual configuration for a TCP server.
        //!   If this TCP port is already bound by another local TCP socket, the bind
        //!   operation fails, unless the "reuse port" option has already been set.
        //!
        //! @param [in] addr Local socket address to bind to.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool bind(const SocketAddress& addr, ReportInterface& report = CERR);

        //!
        //! Get local socket address
        //! @param [out] addr Local socket address of the connection.
        //! @param [in,out] report Where to report error.
        //! @return True on success, false on error.
        //!
        bool getLocalAddress(SocketAddress& addr, ReportInterface& report = CERR);

        //!
        //! Get the underlying socket device handle (use with care).
        //!
        //! This method is reserved for low-level operations and should
        //! not be used by normal applications.
        //!
        //! @return The underlying socket system device handle or file descriptor.
        //! Return @link TS_SOCKET_T_INVALID @endlink if the socket is not open.
        //!
        TS_SOCKET_T getSocket() const
        {
            return _sock;
        }

    protected:
        Mutex _mutex; //!< Mutex protecting this object.

        //!
        //! This virtual method can be overriden by subclasses to be notified of open.
        //! All subclasses should explicitely invoke their superclass' handlers.
        //! @param [in,out] report Where to report error.
        //!
        virtual void handleOpened(ReportInterface& report = CERR) {}

        //!
        //! This virtual method can be overriden by subclasses to be notified of close.
        //! All subclasses should explicitely invoke their superclass' handlers.
        //! @param [in,out] report Where to report error.
        //!
        virtual void handleClosed(ReportInterface& report = CERR) {}

    private:
        TS_SOCKET_T _sock;

        // This method is used by a server to declare that the socket has just become opened.
        void declareOpened(TS_SOCKET_T, ReportInterface& report = CERR);
        friend class TCPServer;

        // Unreachable operations
        TCPSocket(const TCPSocket&) = delete;
        TCPSocket& operator=(const TCPSocket&) = delete;
    };

    //!
    //! Safe pointer to TCPSocket, single-threaded.
    //!
    typedef SafePtr <TCPSocket, NullMutex> TCPSocketPtr;

    //!
    //! Safe pointer to TCPSocket, multi-threaded.
    //!
    typedef SafePtr <TCPSocket, Mutex> TCPSocketPtrMT;
}
