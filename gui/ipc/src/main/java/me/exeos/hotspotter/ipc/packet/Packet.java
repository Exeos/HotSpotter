package me.exeos.hotspotter.ipc.packet;

import me.exeos.hotspotter.ipc.packet.impl.ConnectPacket;
import me.exeos.hotspotter.ipc.packet.impl.MessagePacket;

public abstract class Packet {

    protected final byte id;

    public Packet(byte id) {
        this.id = id;
    }

    public abstract byte[] encode();

    public static Packet decode(byte[] packetBytes) {
        if (packetBytes.length == 0) {
            throw new IllegalStateException("Can't decode empty packet bytes.");
        }

        byte[] payload = new byte[packetBytes.length - 1];
        System.arraycopy(packetBytes, 1, payload, 0, payload.length);

        switch (packetBytes[0]) {
            case 0 -> {
                return new ConnectPacket();
            }
            case 1 -> {
                return new MessagePacket(payload);
            }
            default -> {
                throw new IllegalArgumentException("Payload's id does not match known Packet id");
            }
        }
    }
}
