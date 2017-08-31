#pragma once

class YukiMemoPoolsBlock;

class YukiMemoPools
{
public:
	static YukiMemoPools* getInstance();

	void* mallocInPools(size_t size);
	
	virtual ~YukiMemoPools();
protected:
	YukiMemoPools();

private:
	YukiMemoPoolsBlock* addBlockToTail(size_t size);

	YukiMemoPoolsBlock* m_memoBlocks;
	YukiMemoPoolsBlock* m_currentBlock;
	YukiMemoPoolsBlock* m_tailBlock;
};