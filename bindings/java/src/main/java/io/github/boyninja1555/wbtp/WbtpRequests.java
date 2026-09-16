package io.github.boyninja1555.wbtp;

import io.github.boyninja1555.wbtp.typing.WbtpRequestData;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.nio.charset.StandardCharsets;

import static io.github.boyninja1555.wbtp.typing.WbtpConstants.WBTP_PARAMS_MAX;
import static io.github.boyninja1555.wbtp.typing.WbtpConstants.WBTP_PATH_MAX;

/**
 * Provides utilities for quickly and properly transforming request data.
 */
public final class WbtpRequests {

    /**
     * Sets the path of a request.
     *
     * @param request Request data
     * @param path    New request path
     */
    public static void setPath(@NotNull WbtpRequestData request, @NotNull String path) {
        for (int i = 0; i < WBTP_PATH_MAX; i++) {
            char c = request.path[i];
            if (c == '\0') break;
            request.path[i] = '\0';
        }

        for (int i = 0; i < Math.min(path.length(), WBTP_PATH_MAX); i++) request.path[i] = path.charAt(i);
    }

    /**
     * Sets the params of a request.
     *
     * @param request Request data
     * @param params  New request params
     */
    public static void setParams(@NotNull WbtpRequestData request, @NotNull String params) {
        for (int i = 0; i < WBTP_PARAMS_MAX; i++) {
            if (request.params[i] == '\0') break;
            request.params[i] = '\0';
        }

        for (int i = 0; i < Math.min(params.length(), WBTP_PARAMS_MAX); i++) request.params[i] = params.charAt(i);
    }

    /**
     * Clears the payload of a request.
     *
     * @param request Requests data
     */
    public static void clearPayload(@NotNull WbtpRequestData request) {
        setPayload(request, new byte[0]);
    }

    /**
     * Directly sets the payload of a request.
     *
     * @param request Request data
     * @param payload New request payload
     */
    public static void setPayload(@NotNull WbtpRequestData request, byte[] payload) {
        if (payload.length != request.payload.length) {
            request.payload = payload;
            return;
        }

        System.arraycopy(payload, 0, request.payload, 0, payload.length);
    }

    /**
     * Sets the payload of a request to a string.
     *
     * @param request Request data
     * @param payload Payload string
     */
    public static void setPayloadString(@NotNull WbtpRequestData request, @NotNull String payload) {
        setPayload(request, payload.getBytes(StandardCharsets.UTF_8));
    }

    /**
     * Serializes this request to an output stream.
     *
     * @param request Request data
     * @param out     Output stream
     * @throws IOException When thrown, describes what went wrong during serialization
     */
    public static void serialize(@NotNull WbtpRequestData request, @NotNull OutputStream out) throws IOException {
        var bData = new ByteArrayOutputStream();
        var data = new DataOutputStream(bData);
        data.writeByte(request.type);

        int pathSize = 0;
        while (pathSize < request.path.length && request.path[pathSize] != '\0') pathSize++;
        var path = new String(request.path, 0, pathSize).getBytes(StandardCharsets.UTF_8);
        data.writeInt(path.length);
        data.write(path);

        int paramsSize = 0;
        while (paramsSize < request.params.length && request.params[paramsSize] != '\0') paramsSize++;
        var params = new String(request.params, 0, paramsSize).getBytes(StandardCharsets.UTF_8);
        data.writeInt(params.length);
        data.write(params);

        data.writeInt(request.payload.length);
        data.write(request.payload);

        new DataOutputStream(out).writeInt(bData.size());
        out.write(bData.toByteArray());
    }

    /**
     * Deserializes an input stream into this request.
     *
     * @param request Request data
     * @param in      Input stream
     * @throws IOException When thrown, describes what went wrong during deserialization
     */
    public static void deserialize(@NotNull WbtpRequestData request, @NotNull InputStream in) throws IOException {
        var data = new DataInputStream(in);
        data.readInt();
        request.type = data.readByte();

        var pathSize = data.readInt();
        if (pathSize < 0) throw new IOException("Invalid path size!");
        if (pathSize >= WBTP_PATH_MAX) throw new IOException("Path too long!");
        var path = new byte[pathSize];
        data.readFully(path);
        setPath(request, new String(path, StandardCharsets.UTF_8));

        var paramsSize = data.readInt();
        if (paramsSize < 0) throw new IOException("Invalid params size!");
        if (paramsSize >= WBTP_PARAMS_MAX) throw new IOException("Params too long!");
        var params = new byte[paramsSize];
        data.readFully(params);
        setParams(request, new String(params, StandardCharsets.UTF_8));

        var payloadSize = data.readInt();
        if (payloadSize < 0) throw new IOException("Invalid payload size!");
        var payload = new byte[payloadSize];
        data.readFully(payload);
        setPayload(request, payload);
    }
}
