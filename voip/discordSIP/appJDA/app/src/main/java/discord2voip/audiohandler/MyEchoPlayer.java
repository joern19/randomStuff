package discord2voip.audiohandler;

import java.nio.ByteBuffer;
import java.util.Queue;
import java.util.concurrent.ArrayBlockingQueue;

import discord2voip.Constants;
import net.dv8tion.jda.api.audio.AudioReceiveHandler;
import net.dv8tion.jda.api.audio.AudioSendHandler;
import net.dv8tion.jda.api.audio.CombinedAudio;

public class MyEchoPlayer implements AudioSendHandler, AudioReceiveHandler {

    private Queue<ByteBuffer> buffer = new ArrayBlockingQueue<>(600);

    @Override
    public boolean canReceiveCombined() {
        return true;
    }

    @Override
    public void handleCombinedAudio(CombinedAudio combinedAudio) {
        buffer.add(ByteBuffer.wrap(combinedAudio.getAudioData(1.0)));
    }

    boolean first = true;
    @Override
    public boolean canProvide() {
        return !buffer.isEmpty() || first;
    }

    @Override
    public ByteBuffer provide20MsAudio() {
        if (first) {
            first = false;
            return Constants.get20msSilence(); // Currently we have to play, before we are able to listen. (Discord bug, see README.md)
        }
        return buffer.poll();
    }
}
