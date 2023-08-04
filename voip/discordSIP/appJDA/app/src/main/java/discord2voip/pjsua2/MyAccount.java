package discord2voip.pjsua2;

import java.util.ArrayList;

import org.pjsip.pjsua2.Account;
import org.pjsip.pjsua2.Call;
import org.pjsip.pjsua2.CallInfo;
import org.pjsip.pjsua2.CallOpParam;
import org.pjsip.pjsua2.Endpoint;
import org.pjsip.pjsua2.OnIncomingCallParam;
import org.pjsip.pjsua2.OnRegStateParam;

public class MyAccount extends Account {

  // TODO: check that If a call runs out of scope, it gets dropped.
  ArrayList<Call> activeCalls = new ArrayList<>();
  Endpoint endpoint;

  public MyAccount(Endpoint endpoint) {
    this.endpoint = endpoint;
  }

  @Override
  public void onRegState(OnRegStateParam prm) {
    System.out.println("*** On registration state: " + prm.getCode() + prm.getReason());

    if (prm.getCode() >= 200 && prm.getCode() <= 299) {
      activeCalls.add(MyCall.startCall(this));
    } else {
      System.out.println("onRegState was not successful: " + prm.getStatus());
    }
  }

  @Override
  public void onIncomingCall(OnIncomingCallParam prm) {
    try {
      Call call = MyCall.lookup(prm.getCallId());
      CallOpParam callOpParam = new CallOpParam(true);
      callOpParam.setStatusCode(200);

      callOpParam.getOpt().setAudioCount(1);
      callOpParam.getOpt().setVideoCount(0);

      call.answer(callOpParam);
      CallInfo callInfo = call.getInfo();

      System.out.println("Incoming call from '%s', aka '%s'" + callInfo.getRemoteContact() + callInfo.getRemoteUri());
      System.out.println(callInfo.getMedia().size());

      for (Call activeCall : this.activeCalls) {
        if (!activeCall.isActive())
          activeCalls.remove(activeCall);
      }
      activeCalls.add(call);
    } catch (Exception e) {
      throw new RuntimeException(e);
    }
  }
}
