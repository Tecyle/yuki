#include "stdafx.h"
#include "yuki_memo_pools.h"

#define YUKI_BLOCK_SIZE 4096

static YukiMemoPools* g_instance = nullptr;

class YukiMemoPoolsBlock
{
public:
	YukiMemoPoolsBlock(size_t reservedSize)
	{
		m_buffer = (uint8_t*)malloc(reservedSize);
		m_leftSize = reservedSize;
		m_bufferSize = reservedSize;
		next = nullptr;
	}

	YukiMemoPoolsBlock()
		: YukiMemoPoolsBlock(YUKI_BLOCK_SIZE)
	{
	}

	~YukiMemoPoolsBlock()
	{
		free(m_buffer);
	}

	__inline bool isFull() const { return m_leftSize < 64; }

	void* alloc(size_t size)
	{
		if (size > m_leftSize)
			return nullptr;

		uint8_t* res = m_buffer + (m_bufferSize - m_leftSize);
		m_leftSize -= size;
		return res;
	}

private:
	uint8_t* m_buffer;
	int m_bufferSize;
	int m_leftSize;

public:
	YukiMemoPoolsBlock* next;
};

YukiMemoPools* YukiMemoPools::getInstance()
{
	if (g_instance == nullptr)
		g_instance = new YukiMemoPools;
	return g_instance;
}

void* YukiMemoPools::mallocInPools(size_t size)
{
	// ���������Ѿ��޷���������Ŀ�
	while (m_currentBlock != nullptr)
	{
		if (!m_currentBlock->isFull())
			break;

		m_currentBlock = m_currentBlock->next;
	}
	// ������п鶼�޷��������䣬�������µĿ�
	if (m_currentBlock == nullptr)
	{
		m_currentBlock = addBlockToTail(size);
	}
	// �����ڵ�ǰ�鼰�����������������ڴ�
	YukiMemoPoolsBlock* p = m_currentBlock;
	while (p)
	{
		void* res = p->alloc(size);
		if (res != nullptr)
			return res;

		p = p->next;
	}
	// �����п鶼�޷�����ʱ�������¿�
	p = addBlockToTail(size);
	return p->alloc(size);
}

YukiMemoPools::YukiMemoPools()
{
	m_memoBlocks = nullptr;
	m_tailBlock = nullptr;
	m_currentBlock = nullptr;
}

YukiMemoPools::~YukiMemoPools()
{
	YukiMemoPoolsBlock* p = m_memoBlocks;
	YukiMemoPoolsBlock* pNext = p ? p->next : nullptr;
	while (p)
	{
		delete p;
		p = pNext;
		pNext = p ? p->next : nullptr;
	}
}

YukiMemoPoolsBlock* YukiMemoPools::addBlockToTail(size_t size)
{
	size = size > YUKI_BLOCK_SIZE ? size : YUKI_BLOCK_SIZE;
	YukiMemoPoolsBlock* block = new YukiMemoPoolsBlock(size);

	if (m_tailBlock == nullptr)
	{
		assert(m_memoBlocks == nullptr);
		m_memoBlocks = m_tailBlock = block;
	}
	else
	{
		m_tailBlock->next = block;
		m_tailBlock = block;
	}

	return block;
}

