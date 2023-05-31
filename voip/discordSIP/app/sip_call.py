import pjsua2 as pj
import sip_audio
import os

test = None

class Call(pj.Call):

    ep: pj.Endpoint = None

    def __init__(self, acc, call_id=pj.PJSUA_INVALID_ID):
        super().__init__(acc, call_id)

    def hangup(self):
        prm = pj.CallOpParam(True)
        prm.statusCode = pj.PJSIP_SC_DECLINE
        super().hangup(prm)

    def onCallState(self, prm):
        print("\n\n\n-------------------------------------------------------- on_call_state ------------------------------\n\n\n")
        print("on_call_state: ", prm)
    
    def onCallMediaState(self, prm):
        global test
        print("\n\n\n-------------------------------------------------------- on_media_state------------------------------\n\n\n")
        ci = self.getInfo()
        print(len(ci.media))
        for mi in ci.media:
            if mi.type == pj.PJMEDIA_TYPE_AUDIO and mi.status == pj.PJSUA_CALL_MEDIA_ACTIVE: # todo: on hold? can that break something?
                m = self.getMedia(mi.index)
                am = pj.AudioMedia.typecastFromMedia(m)

                #sip_audio.createWavPlayer().startTransmit(am)
                am.startTransmit(am)
                #break

        print("on_media_state: ", prm)

    def onDtmfDigit(self, digits):
        print("on_dtmf_digit: ", digits.digit)

def start_call(acc) -> Call:
    call = Call(acc)
    prm = pj.CallOpParam(True)
    prm.opt.audioCount = 1
    prm.opt.videoCount = 0

    try:
        call.makeCall(f"sip:{os.environ['PHONE_NUMBER']}@192.168.1.1", prm)
        #call.makeCall("sip:**623@192.168.1.1", prm)
    except Exception as e:
        print("!!!!Error!!!!")
        print(e.info())
        print("\n\n\n")
    print("start_call finished!")
    return call

if __name__ == "__main__":
    import sip
