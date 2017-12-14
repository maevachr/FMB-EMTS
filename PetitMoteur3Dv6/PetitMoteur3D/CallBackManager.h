#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>
#include "GameObject.h"
#include "BlackBoard.h"
#include "SoundManager.h"

using namespace std;

namespace PM3D
{
	class ICallBack {
	protected:
		float basetime;
		float time;
	public:
		bool Update(float tempsEcoule) {
			time -= tempsEcoule;
			if (time < 0) 
			{
				Trigger();
				return true;
			}
			return false;
		}
		virtual void Trigger() = 0;
	};

	class UnspawnCallBack : public ICallBack {
		GameObject* go;
	public:
		UnspawnCallBack(GameObject* go, float _time) : go{ go }
		{
			basetime = _time;
			time = _time;
		}

		virtual void Trigger() override {
			go->OnUnspawn();
		}
	};

// 	class StopSoundCallBack : public ICallBack 
// 	{
// 		SoundManager::SoundType type;
// 	public:
// 		StopSoundCallBack(SoundManager::SoundType _type, float _time) : type{ _type }
// 		{
// 			basetime = _time;
// 			time = _time;
// 		}
// 
// 		virtual void Trigger() override {
// 			SoundManager::GetInstance().PauseSound(type);
// 		}
// 	};

	class CallBackManager
	{
	private:
		CallBackManager() = default;
		~CallBackManager() = default;
	public:
		CallBackManager(const CallBackManager&) = delete;
		CallBackManager operator=(const CallBackManager&) = delete;

	public:
		static CallBackManager& GetInstance()
		{
			static CallBackManager cm;
			return cm;
		}

	private:
		vector<ICallBack*> callBack;
	public:
		void AddCallBack(ICallBack* cb)
		{
			callBack.push_back(cb);
		}

	public:
		void UpdateTime(float tempsEcoule) {
			vector<vector<ICallBack*>::iterator> callBackRemoved;
			for (auto it = callBack.begin(); it != callBack.end(); ++it)
			{
				if ((*it)->Update(tempsEcoule)) 
				{
					callBackRemoved.push_back(it);
				}
			}

			reverse(begin(callBackRemoved), end(callBackRemoved));
			for_each(begin(callBackRemoved), end(callBackRemoved), [&](vector<ICallBack*>::iterator cb) {
				callBack.erase(cb);
			});

		}
	};
}