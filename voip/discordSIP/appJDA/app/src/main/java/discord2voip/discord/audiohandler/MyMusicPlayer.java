package discord2voip.discord.audiohandler;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;

import discord2voip.discord.Constants;
import net.dv8tion.jda.api.audio.AudioSendHandler;

public class MyMusicPlayer implements AudioSendHandler {

    private Integer index = 0;
    private ArrayList<ByteBuffer> music = new ArrayList<>();    

    public MyMusicPlayer() throws IOException {
        // Load the audio into memory
        // ffmpeg -i music.mp3 -f s16be -ac 2 -ar 48k music.raw
        // ffplay -f s16be -ac 2 -ar 48k audio.raw
        Path path = Paths.get("music.raw");
        byte[] data = Files.readAllBytes(path);
        System.out.println(data.length + " bytes read. The audio is " + String.valueOf(data.length * 8 / Constants.BITRATE) + " seconds long.");

        ByteBuffer bb = Constants.get20msSilence();
        for (byte b : data) {
            bb.put(b);
            if (bb.position() == bb.limit()) {
                music.add(bb);
                bb.flip();
                bb = Constants.get20msSilence();
            }
        }
    }

    @Override
    public boolean canProvide() {
        return true;
    }

    @Override
    public ByteBuffer provide20MsAudio() {
        ByteBuffer result = music.get(index);
        index++;
        if (index == music.size()) {
            index = 0;
        }
        return result;
    }
    
}
