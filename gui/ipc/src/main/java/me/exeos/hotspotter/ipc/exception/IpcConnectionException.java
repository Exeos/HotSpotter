package me.exeos.hotspotter.ipc.exception;

public class IpcConnectionException extends RuntimeException {

    public IpcConnectionException(String message) {
        super(message);
    }

    public IpcConnectionException(String message, Throwable cause) {
        super(message, cause);
    }

    public IpcConnectionException(Throwable cause) {
        super(cause);
    }
}
