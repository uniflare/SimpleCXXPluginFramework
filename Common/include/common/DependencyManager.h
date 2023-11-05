// Copyright 2023 Jeremy Wade
#pragma once

#include "Guid.h"
#include <cassert>
#include <list>
#include <map>
#include <stack>
#include <vector>
#include <optional>
#include <functional>

struct SDependencyItem
{
#ifdef _DEBUG
	std::string name;
#endif
	SGuid guid;
};

using TDependencyVisitor = void(const SDependencyItem& item);

class CDependencyResolver
{
	struct SDependencyLink
	{
		SGuid parent;
		SGuid child;
	};

	struct SDependencyChainItem
	{
		const SDependencyItem* pItem;
		std::vector<SDependencyChainItem*> parents;
		std::vector<SDependencyChainItem*> children;
		bool wasVisited;
	};

public:
	CDependencyResolver()
		: m_items()
		, m_links()
	{}

#ifdef _DEBUG
	void RegisterItem(const SGuid& guid, std::string&& name);
#else
	void RegisterItem(const SGuid& guid);
#endif
	void AddDependency(const SGuid& parent, const SGuid& child);

	bool ItemExists(const SGuid& guid);
	bool LinkExists(const SGuid& parent, const SGuid& child);

	// Dependencies -> Dependants
	void VisitTopDown(std::function<TDependencyVisitor> visitor) const;

	// Dependants -> Dependencies
	void VisitBottomUp(std::function<TDependencyVisitor> visitor) const;

protected:

	std::list<SDependencyChainItem> BuildChains() const;

	template<bool TopDown>
	void VisitLinks(std::function<TDependencyVisitor> visitor, SDependencyChainItem& chainItem) const
	{
		std::vector<SDependencyChainItem*>& forwardList = TopDown ? chainItem.parents : chainItem.children;
		std::vector<SDependencyChainItem*>& backwardList = TopDown ? chainItem.children : chainItem.parents;

		for (SDependencyChainItem* pParent : forwardList)
			if (!pParent->wasVisited)
				VisitLinks<TopDown>(visitor, *pParent);

		if (!chainItem.wasVisited)
		{
			visitor(*chainItem.pItem);
			chainItem.wasVisited = true;
		}

		for (SDependencyChainItem* pChild : backwardList)
			VisitLinks<TopDown>(visitor, *pChild);
	}

	// Depth First Search to check for cyclical dependencies
	bool HaveCyclicDependency(SDependencyChainItem* node, std::map<SDependencyChainItem*, bool>& visited) const;

private:

	std::vector<SDependencyItem> m_items;
	std::vector<SDependencyLink> m_links;

};