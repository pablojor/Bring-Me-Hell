#pragma once

class Money
{
private:
	int _bank = 0, _wallet = 0;

public:
	inline void restartWallet() { _wallet = 0; }
	inline void storeWallet() { _bank += _wallet; }
	inline void store(int s) { _wallet += s; }

	bool spend(int n);
	inline int getWallet() const { return _wallet; }
};