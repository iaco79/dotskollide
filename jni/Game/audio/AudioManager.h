#ifndef GAUDIOMANAGER_CLASS
#define GAUDIOMANAGER_CLASS



class AudioItem
{


public:

	enum AUDIO_CHUNKS
	{
		CHUNK_BLAST1=0,
		CHUNK_BLAST2,
		CHUNK_PULL,
		CHUNK_COLLECT1,
		CHUNK_COLLECT2,		
		CHUNK_LEVELUP,
		CHUNK_LOST,
		CHUNK_COUNT
	};

	enum AUDIO_STATES
	{
		STOPPED,
		TOPLAY,
		PLAYING,
		TOSTOP,
		RESET
	};


	enum AUDIO_FLAGS
	{
		PLAYONCE = (1 << 0),
		PLAYKEEP = (1 << 1),
	};


	int mId;
	int mLoop;
	int mState;
	int mChunkId;
	int mLastChunkId;
	int mChannel;
	int mFlags;

	AudioItem():mId(-1),mLoop(0),mState(STOPPED), mChannel(-1), mChunkId(-1),mLastChunkId(-1),mFlags(0) {;}
	AudioItem(int loop,int state,int chunkId):mId(0),mLoop(loop),mState(state), mChannel(-1), mChunkId(chunkId),mLastChunkId(-1),mFlags(0) {;}

};

class AudioManager
{

private:
	static AudioManager* gInstance;
	static bool gInstanceFlag;
	

	AudioManager() { 
		mMusicEnabled=false;
		mSoundEnabled=false;
		mMusicPlaying=false;

		mMusic = NULL;}

public:

	static int gChunkIndexes[AudioItem::CHUNK_COUNT];

	static AudioManager* getInstance();

	bool initialize();
	void shutDown();

	void update();

	int  playSound(AudioItem audio);
	int  playSoundNow(AudioItem audio);

	
	//void resetSound(AudioItem item);
	//void updateSound(AudioItem item);
	void stopSound(AudioItem audio);
	int getSoundIndexById(int id);

	void playMusic();
	void stopMusic();

	void clearChunks();
	int loadChunk(irr::core::stringc fileName);
	
	void loadMusic(irr::core::stringc fileName);

	void pauseAudio();
	void resumeAudio();
	void enableSound(bool enable);
	void enableMusic(bool enable);
	

protected:
	bool mMusicEnabled;
	bool mSoundEnabled;
	bool mMusicPlaying;

	irr::core::array<AudioItem> mAudioList;
	irr::core::array<void*> mChunkList;
	void *mMusic;

};

#endif