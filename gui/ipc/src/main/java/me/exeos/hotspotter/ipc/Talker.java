package me.exeos.hotspotter.ipc;

import me.exeos.hotspotter.ipc.exception.IpcConnectionException;
import me.exeos.hotspotter.ipc.packet.Packet;
import org.zeromq.SocketType;
import org.zeromq.ZContext;
import org.zeromq.ZMQ;

public class Talker implements AutoCloseable {

    private final ZMQ.Socket requestSocket;
    private final ZContext context;

    public Talker() throws RuntimeException {
        try {
            context = new ZContext();

            requestSocket = context.createSocket(SocketType.REQ);
            requestSocket.connect("tcp://localhost:3247");
        } catch (Exception e) {
            throw new IpcConnectionException("Failed to establish connection.", e.getCause());
        }
    }

    public Packet request(Packet request) {
        requestSocket.send(request.encode(), 0);

        return Packet.decode(requestSocket.recv(0));
    }

    @Override
    public void close() {
        requestSocket.close();
        context.close();
    }
}
