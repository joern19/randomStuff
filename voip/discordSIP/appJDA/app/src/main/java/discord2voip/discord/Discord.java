package discord2voip.discord;

import java.util.EnumSet;

import discord2voip.discord.audiohandler.MyEchoPlayer;
import net.dv8tion.jda.api.JDA;
import net.dv8tion.jda.api.JDABuilder;
import net.dv8tion.jda.api.audio.AudioReceiveHandler;
import net.dv8tion.jda.api.audio.AudioSendHandler;
import net.dv8tion.jda.api.entities.channel.concrete.VoiceChannel;
import net.dv8tion.jda.api.managers.AudioManager;
import net.dv8tion.jda.api.requests.GatewayIntent;

public class Discord {

    public static final Object CURRENT_AUDIO_HANDLER = new MyEchoPlayer();

    private static JDA login() throws InterruptedException {
        String botToken = System.getenv("DISCORD_BOT_TOKEN");
        JDA jda = JDABuilder.createDefault(botToken, EnumSet.allOf(GatewayIntent.class)).build();
        System.out.println("Logging in...");
        jda.awaitReady();
        String username = jda.getSelfUser().getName();
        System.out.println("Logged in as " + username);
        return jda;
    }

    public static void start() throws Exception {
        JDA jda = login();
        VoiceChannel voiceChannel = jda.getVoiceChannelById("595279323858993172");
        AudioManager audioManager = voiceChannel.getGuild().getAudioManager();

        if (CURRENT_AUDIO_HANDLER instanceof AudioSendHandler) {
            audioManager.setSendingHandler((AudioSendHandler) CURRENT_AUDIO_HANDLER);
        }

        if (CURRENT_AUDIO_HANDLER instanceof AudioReceiveHandler) {
            audioManager.setReceivingHandler((AudioReceiveHandler) CURRENT_AUDIO_HANDLER);
        }
        audioManager.openAudioConnection(voiceChannel);
    }
}
