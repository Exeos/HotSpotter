package me.exeos.hotspotter.ipc.packet.impl;

import me.exeos.hotspotter.ipc.helper.ByteHelper;
import me.exeos.hotspotter.ipc.packet.Packet;

import java.nio.charset.StandardCharsets;

public class MessagePacket extends Packet {

    private final String message;

    public MessagePacket(String message) {
        super((byte) 1);
        this.message = message;
    }

    public MessagePacket(byte[] message) {
        super((byte) 0);
        this.message = new String(message, StandardCharsets.UTF_8);
    }

    @Override
    public byte[] encode() {
        return ByteHelper.concat(super.id, message.getBytes(StandardCharsets.UTF_8));
    }
}
