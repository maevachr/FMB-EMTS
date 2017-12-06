#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>
#include "GameObject.h"

namespace PM3D
{
	class ICallBack {
	protected:
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
			time = _time;
		}

		virtual void Trigger() override {
			go->OnUnspawn();
		}
	};


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
		std::vector<ICallBack*> callBack;

	public:
		void AddCallBack(ICallBack* cb)
		{
			callBack.push_back(cb);
		}
	public:
		void UpdateTime(float tempsEcoule) {
			std::remove_if(begin(callBack), end(callBack), [tempsEcoule](ICallBack* cb) -> bool {
				return cb->Update(tempsEcoule);
			});
		}
	};
}