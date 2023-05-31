import pjsua2 as pj

def createWavPlayer() -> pj.AudioMediaPlayer:
    player = pj.AudioMediaPlayer()
    try:
        player.createPlayer("/app/test.wav")
    except Exception as e:
        print(e)
    return player

def getSpeaker() -> pj.AudioMedia:
    return pj.Endpoint.instance().audDevManager().getPlaybackDevMedia()
