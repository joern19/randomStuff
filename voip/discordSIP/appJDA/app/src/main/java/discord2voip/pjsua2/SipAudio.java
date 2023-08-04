package discord2voip.pjsua2;

import org.pjsip.pjsua2.AudioMedia;
import org.pjsip.pjsua2.AudioMediaPlayer;
import org.pjsip.pjsua2.Endpoint;

public class SipAudio {

    public AudioMediaPlayer createWavPlayer() {
        AudioMediaPlayer player = new AudioMediaPlayer();
        try {
            player.createPlayer("/app/test.wav");
        } catch (Exception e) {
            e.printStackTrace();
        }
        return player;
    }

    public AudioMedia getSpeaker() {
        try {
            return Endpoint.instance().audDevManager().getPlaybackDevMedia();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
