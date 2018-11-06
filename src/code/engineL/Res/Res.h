
#include <atomic>

enum class ResState
{
	UNLOAD,
	LOADING,
	RELOADING,
	DONE,
	FAILED,
};

class CResState
{
public:
	CResState():m_State(ResState::UNLOAD){}
	ResState GetState() const
	{
		return m_State.load(std::memory_order_acquire);
	}
	void SetState(ResState state)
	{
		m_State.store(state, std::memory_order_release);
	}
private:
	std::atomic<ResState> m_State;
};

/*
Res 类需要提供一个默认的构造函数, 和一个静态方法 Load(std::shared_ptr<Res>, const ResKey&)
*/
