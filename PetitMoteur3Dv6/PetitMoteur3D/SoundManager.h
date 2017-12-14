#pragma once
#include <fmod.hpp>

namespace PM3D
{

	using namespace FMOD;

	class SoundManager
	{
	private:
		SoundManager() = default;
		~SoundManager() = default;
	public:
		SoundManager(const SoundManager&) = delete;
		SoundManager operator=(const SoundManager&) = delete;

	public:
		static SoundManager& GetInstance()
		{
			static SoundManager sm;
			return sm;
		}

	public:
		enum SoundType {
			INTRO,
			COUNTDOWN,
			ENGINE, 
			EXPLODING_CRATE, 
			MUSIC, 
			BOOST,
			CROWD_COUNTDOWN,
			COUNT
		};
	private:
		System		*system;
		Sound		*sound[COUNT];
		Channel		*channel[COUNT];
		
		float		engineFrequency;
		float		engineVolume;

	public:
		void Init() 
		{
			FMOD::System_Create(&system);
			system->init(32, FMOD_INIT_NORMAL, nullptr);

			system->createSound("intro.mp3", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[INTRO]);
			system->createSound("countdown.wav", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[COUNTDOWN]);
			system->createSound("engine.wav", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[ENGINE]);
			system->createSound("wood_explosion.mp3", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[EXPLODING_CRATE]);
			system->createSound("dynamite.mp3", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[MUSIC]);
			system->createSound("boost.wav", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[BOOST]);
			system->createSound("crowd_countdown.wav", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[CROWD_COUNTDOWN]);

			sound[ENGINE]->setLoopCount(-1);
			sound[BOOST]->setLoopCount(-1);
			sound[CROWD_COUNTDOWN]->setLoopCount(0);
			sound[COUNTDOWN]->setLoopCount(0);
			sound[EXPLODING_CRATE]->setLoopCount(0);

			//Start Loops
			system->playSound(FMOD_CHANNEL_FREE, sound[BOOST], true, &channel[BOOST]);
			system->playSound(FMOD_CHANNEL_FREE, sound[ENGINE], true, &channel[ENGINE]);
			system->playSound(FMOD_CHANNEL_FREE, sound[MUSIC], true, &channel[MUSIC]);
			system->playSound(FMOD_CHANNEL_FREE, sound[INTRO], true, &channel[INTRO]);

			//Get the engine base parameter
			channel[ENGINE]->getFrequency(&engineFrequency);
			channel[ENGINE]->getVolume(&engineVolume);

			//Adapt Sound volume of music
			float musicVolume;
			channel[MUSIC]->getVolume(&musicVolume);
			channel[MUSIC]->setVolume(musicVolume * 0.8);

			float countdownVolume;
			channel[COUNTDOWN]->getVolume(&countdownVolume);
			channel[COUNTDOWN]->setVolume(countdownVolume* 1.2);
		}

		//Dont use this for music | engine | boost which are loops
		void PlaySoundEffect(SoundType id) {
			system->playSound(FMOD_CHANNEL_FREE, sound[id], false, &channel[id]);
		}
		void PauseSound(SoundType id) {
			channel[id]->setPaused(true);
		}
		void ContinueSound(SoundType id) {
			channel[id]->setPaused(false);
		}
		void PauseAllGameSounds() {
			for (int i = 1; i < SoundType::COUNT; ++i) {
				channel[i]->setPaused(true);
			}
		}
		void ContinueAllGameSounds() {
			for (int i = 1; i < SoundType::COUNT; ++i) {
				channel[i]->setPaused(false);
			}
		}

		void Engine(float kmh) {
			channel[ENGINE]->setFrequency(engineFrequency + engineFrequency * (3.f * kmh / 200.f));
			channel[ENGINE]->setVolume(engineVolume + engineVolume * (1.3f * kmh / 200.f));
		}

		void Clean() 
		{
			for (int i = 0; i < SoundType::COUNT; ++i)
			{
				sound[i]->release();
			}
			system->close();
			system->release();
		}
		
	};
}
