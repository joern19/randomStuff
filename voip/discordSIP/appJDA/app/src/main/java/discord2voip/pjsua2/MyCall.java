package discord2voip.pjsua2;

import org.pjsip.pjsua2.Account;
import org.pjsip.pjsua2.AudioMedia;
import org.pjsip.pjsua2.Call;
import org.pjsip.pjsua2.CallInfo;
import org.pjsip.pjsua2.CallOpParam;
import org.pjsip.pjsua2.OnCallMediaStateParam;
import org.pjsip.pjsua2.OnCallStateParam;
import org.pjsip.pjsua2.pjmedia_type;
import org.pjsip.pjsua2.pjsip_status_code;
import org.pjsip.pjsua2.pjsua_call_media_status;

public class MyCall extends Call {

    public MyCall(Account acc) {
        super(acc);
    }

    public void hangup() throws Exception {
        CallOpParam prm = new CallOpParam(true);
        prm.setStatusCode(pjsip_status_code.PJSIP_SC_DECLINE);
        hangup(prm);
    }

    @Override
    public void onCallState(OnCallStateParam prm) {
        System.out.println("\n\n\n-------------------------------------------------------- on_call_state ------------------------------\n\n\n");
        System.out.println("on_call_state: " + prm.toString());
    }

    @Override
    public void onCallMediaState(OnCallMediaStateParam prm) {
        try {
            // global test
            System.out.println("\n\n\n-------------------------------------------------------- on_media_state------------------------------\n\n\n");
            CallInfo ci = this.getInfo();
            System.out.println(ci.getMedia().size());
            ci.getMedia().forEach(m -> {
                // todo: on hold? can that break something?
                if (m.getType() == pjmedia_type.PJMEDIA_TYPE_AUDIO && m.getStatus() == pjsua_call_media_status.PJSUA_CALL_MEDIA_ACTIVE) {
                    AudioMedia am = AudioMedia.typecastFromMedia(this.getMedia(m.getIndex()));
                    try {
                        am.startTransmit(am);
                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                }
            });

            System.out.println("on_media_state: " + prm.toString());
            
    /*
        aud_med = self.getAudioMedia(-1)
        pcm_capture = pj.AudioMediaCapture()
        pcm_capture.createMediaCapture(ci.id)
        aud_med.startTransmit(pcm_capture)
        pcm_stream = pj.AudioMediaStream()
        pcm_stream.createMediaStream(ci.id)
        pcm_stream.startTransmit(aud_med)


        f = open('output.lpcm', 'wb')
        f2 = open('hw.raw', 'rb')
        hwraw = f2.read()
        n = 320
        [pcm_stream.putFrame(hwraw[i:i+320]) for i in range(0, len(hwraw), n)]

        for i in range(10):
            time.sleep(1)


            data = pcm_capture.getFrames()
            print(type(data))
            print("===== fetch:",len(data))

            if data:
                f.write(data)
        f.close()*/

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static MyCall startCall(Account account) {
        MyCall call = new MyCall(account);
        CallOpParam prm = new CallOpParam(true);
        prm.getOpt().setAudioCount(1);
        prm.getOpt().setVideoCount(0);

        try {
            //call.makeCall(f"sip:{os.environ['PHONE_NUMBER']}@192.168.1.1", prm)
            call.makeCall("sip:**610@192.168.1.1", prm);
        } catch (Exception e) {
            System.out.println("!!!!Error!!!!");
            e.printStackTrace();
        }
        System.out.println("startCall finished.");
        return call;
    }
}
