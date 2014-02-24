// ----------------------------------------------------------------------------
//
// File: prGui.h
//
//      Description     - Contains GUI listener class
//      Author          - Paul Michael McNab.
//      Copyright       - Copyright Paul Michael McNab. All rights reserved.
//
// Disclaimer:
//
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
//      TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//      PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//      CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//      EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//      PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//      PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//      LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//      NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ----------------------------------------------------------------------------


#ifndef _GUILISTENER_H
#define _GUILISTENER_H


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
class prGuiListener
{
public:

    prGuiListener() {}
    ~prGuiListener() {}

    virtual void OnPressed() = 0;
    virtual void OnMove() = 0;
    virtual void OnReleased() = 0;
};


#endif//_GUILISTENER_H
