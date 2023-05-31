import pjsua2 as pj
from sip_call import Call
from typing import List
from sip_call import start_call

class Account(pj.Account):

    def __init__(self, ep):
        super().__init__()
        # TODO: create a virtual audio device: discord -> pjsua2 and pjsua2 -> discord
        self.ep = ep

    # If a call runs out of scope, it gets dropped.
    activeCalls: List[Call] = []

    def onIncomingCall(self, call):
        print("Incoming call from ", call.info().remote_uri)

    def onRegState(self, prm):
        if (prm.code >= 200 and prm.code <= 299):
            self.activeCalls.append(start_call(self))
        else:
            print("onRegState was not successful: ", prm.status)

    def onIncomingCall(self, prm):
        c = Call(self, call_id=prm.callId)
        call_prm = pj.CallOpParam(True)
        call_prm.statusCode = 200
        call_prm.opt.audioCount = 1
        call_prm.opt.videoCount = 0
        c.answer(call_prm)
        ci = c.getInfo()
        print("Incoming call from '%s', aka '%s'" % (ci.remoteContact, ci.remoteUri))
        print(len(ci.media))

        for activeCall in self.activeCalls:
            if not activeCall.isActive():
                self.activeCalls.remove(activeCall)
        self.activeCalls.append(c)
