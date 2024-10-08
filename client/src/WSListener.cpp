
#include "WSListener.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WSListener

void WSListener::onPing(const WebSocket& socket, const oatpp::String& message)
{
  OATPP_LOGd(TAG, "onPing");
  std::lock_guard<std::mutex> lock(m_writeMutex);
  socket.sendPong(message);
}

void WSListener::onPong(const WebSocket& socket, const oatpp::String& message)
{
  OATPP_LOGd(TAG, "onPong");
}

void WSListener::onClose(const WebSocket& socket, v_uint16 code, const oatpp::String& message)
{
  OATPP_LOGd(TAG, "onClose code=%d", code);
}

void WSListener::readMessage(const WebSocket& socket,
                             v_uint8 opcode,
                             p_char8 data,
                             oatpp::v_io_size size)
{

  if(size == 0) { // message transfer finished

    auto wholeMessage = m_messageBuffer.toString();
    m_messageBuffer.setCurrentPosition(0);

    OATPP_LOGd(TAG, "on message received '%s'", wholeMessage->c_str());

    /* Send message in reply */
    //std::lock_guard<std::mutex> lock(m_writeMutex);
    //socket.sendOneFrameText( "Hello from oatpp!: " + wholeMessage);
  }
  else if(size > 0) { // message frame received
    m_messageBuffer.writeSimple(data, size);
  }
}
