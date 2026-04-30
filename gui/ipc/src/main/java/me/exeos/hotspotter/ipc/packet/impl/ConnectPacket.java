package me.exeos.hotspotter.ipc.packet.impl;

import me.exeos.hotspotter.ipc.packet.Packet;

public class ConnectPacket extends Packet {

    public ConnectPacket() {
        super((byte) 0);
    }

    @Override
    public byte[] encode() {
        return new byte[] { super.id };
    }
}
