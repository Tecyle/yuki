#include "stdafx.h"
#include "yuki_internal_types.h"

YukiRegion::YukiRegion(const YukiRegion* region)
{
	m_regionType = region->m_regionType;
	m_sLn		 = region->m_sLn;
	m_sCol		 = region->m_sCol;
	m_eLn		 = region->m_eLn;
	m_eCol		 = region->m_eCol;
	m_sCh		 = region->m_sCh;
	m_indent	 = region->m_indent;
}
