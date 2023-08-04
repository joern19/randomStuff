package discord2voip.discord;

import java.nio.ByteBuffer;

public class Constants {
    public static final Integer BITRATE = 1536000;
    public static final Integer BITS_PER_20MS = BITRATE / 50; // there are 20ms * 50 = 1s

    public static final ByteBuffer get20msSilence() {
        return ByteBuffer.allocate(BITS_PER_20MS / 8);
    }

}
