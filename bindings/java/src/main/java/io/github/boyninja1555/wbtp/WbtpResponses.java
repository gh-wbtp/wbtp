package io.github.boyninja1555.wbtp;

import io.github.boyninja1555.wbtp.typing.WbtpResponseData;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.nio.charset.StandardCharsets;

import static io.github.boyninja1555.wbtp.typing.WbtpConstants.WBTP_PARAMS_MAX;

/**
 * Provides utilities for quickly and properly transforming response data.
 */
public final class WbtpResponses {

    /**
     * Sets the params of a response.
     *
     * @param response Response data
     * @param params   New response params
     */
    public static void setParams(@NotNull WbtpResponseData response, @NotNull String params) {
        for (int i = 0; i < WBTP_PARAMS_MAX; i++) {
            if (response.params[i] == '\0') break;
            response.params[i] = '\0';
        }

        for (int i = 0; i < Math.min(params.length(), WBTP_PARAMS_MAX); i++) response.params[i] = params.charAt(i);
    }

    /**
     * Clears the payload of a response.
     *
     * @param response Response data
     */
    public static void clearPayload(@NotNull WbtpResponseData response) {
        setPayload(response, new byte[0]);
    }

    /**
     * Directly sets the payload of a response.
     *
     * @param response Response data
     * @param payload  New response payload
     */
    public static void setPayload(@NotNull WbtpResponseData response, byte[] payload) {
        if (payload.length != response.payload.length) {
            response.payload = payload;
            return;
        }

        System.arraycopy(payload, 0, response.payload, 0, payload.length);
    }

    /**
     * Sets the payload of a response to a string.
     *
     * @param response Response data
     * @param payload  Payload string
     */
    public static void setPayloadString(@NotNull WbtpResponseData response, @NotNull String payload) {
        setPayload(response, payload.getBytes(StandardCharsets.UTF_8));
    }

    /**
     * Serializes this response to an output stream.
     *
     * @param response Response data
     * @param out      Output stream
     * @throws IOException When thrown, describes what went wrong during serialization
     */
    public static void serialize(@NotNull WbtpResponseData response, @NotNull OutputStream out) throws IOException {
        var bData = new ByteArrayOutputStream();
        var data = new DataOutputStream(bData);
        data.writeByte(response.type);

        int paramsSize = 0;
        while (paramsSize < response.params.length && response.params[paramsSize] != '\0') paramsSize++;
        var params = new String(response.params, 0, paramsSize).getBytes(StandardCharsets.UTF_8);
        data.writeShort(params.length);
        data.write(params);

        data.writeInt(response.payload.length);
        data.write(response.payload);

        new DataOutputStream(out).writeInt(bData.size());
        out.write(bData.toByteArray());
    }

    /**
     * Deserializes an input stream into this response.
     *
     * @param response Response data
     * @param in       Input stream
     * @throws IOException When thrown, describes what went wrong during deserialization
     */
    public static void deserialize(@NotNull WbtpResponseData response, @NotNull InputStream in) throws IOException {
        var data = new DataInputStream(in);
        data.readInt();
        response.type = data.readByte();

        var paramsSize = data.readUnsignedShort();
        if (paramsSize >= WBTP_PARAMS_MAX) throw new IOException("Params too long!");
        var params = new byte[paramsSize];
        data.readFully(params);
        setParams(response, new String(params, StandardCharsets.UTF_8));

        var payloadSize = data.readInt();
        if (payloadSize < 0) throw new IOException("Invalid payload size!");
        var payload = new byte[payloadSize];
        data.readFully(payload);
        setPayload(response, payload);
    }
}
