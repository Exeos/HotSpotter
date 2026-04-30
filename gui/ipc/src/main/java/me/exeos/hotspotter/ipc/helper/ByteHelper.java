package me.exeos.hotspotter.ipc.helper;

import java.util.Objects;

public class ByteHelper {

    public static byte[] concat(Object... parts) {
        Objects.requireNonNull(parts, "parts");

        int size = 0;
        for (Object p : parts) {
            if (p instanceof byte[] a) {
                size += a.length;
            } else if (p instanceof Byte) {
                size += 1;
            } else {
                throw new IllegalArgumentException(
                        "Expected byte[] or Byte, got: " + (p == null ? "null" : p.getClass().getName()));
            }
        }

        byte[] out = new byte[size];
        int pos = 0;
        for (Object p : parts) {
            if (p instanceof byte[] a) {
                System.arraycopy(a, 0, out, pos, a.length);
                pos += a.length;
            } else {
                out[pos++] = (Byte) p;
            }
        }

        return out;
    }
}
