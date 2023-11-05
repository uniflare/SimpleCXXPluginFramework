// Copyright 2023 Jeremy Wade
#include "DependencyManager.h"
#include <stack>

#ifdef _DEBUG
void CDependencyResolver::RegisterItem(const SGuid& guid, std::string&& name)
{
	assert(!ItemExists(guid));
	m_items.emplace_back(SDependencyItem{ std::move(name), guid });
}
#else
void CDependencyResolver::RegisterItem(const SGuid& guid)
{
	m_items.emplace_back(SDependencyItem{ guid });
}
#endif

void CDependencyResolver::AddDependency(const SGuid& parent, const SGuid& child)
{
#ifdef _DEBUG
	assert(parent != child);
	assert(ItemExists(parent));
	assert(ItemExists(child));
	assert(!LinkExists(parent, child));
	assert(!LinkExists(child, parent)); // Most likely a circular dependency between 2 items
#endif
	m_links.emplace_back(SDependencyLink{ parent, child });
}

bool CDependencyResolver::ItemExists(const SGuid& guid)
{
	auto it = std::find_if(m_items.cbegin(), m_items.cend(), [guid](const SDependencyItem& item)
	{
		return item.guid == guid;
	});

	return it != m_items.cend();
}

bool CDependencyResolver::LinkExists(const SGuid& parent, const SGuid& child)
{
	auto it = std::find_if(m_links.cbegin(), m_links.cend(), [parent, child](const SDependencyLink& link)
	{
		return link.parent == parent && link.child == child;
	});

	return it != m_links.cend();
}

void CDependencyResolver::VisitTopDown(std::function<TDependencyVisitor> visitor) const
{
	auto chainItems = BuildChains();
	VisitLinks<true>(visitor, chainItems.front());
}

void CDependencyResolver::VisitBottomUp(std::function<TDependencyVisitor> visitor) const
{
	auto chainItems = BuildChains();
	VisitLinks<false>(visitor, chainItems.front());
}

std::list<CDependencyResolver::SDependencyChainItem> CDependencyResolver::BuildChains() const
{
#ifdef _DEBUG
	static SDependencyItem rootItem{ "_root_", SGuid() };
#else
	static SDependencyItem rootItem{ SGuid() };
#endif

	std::list<SDependencyChainItem> chainItems{ SDependencyChainItem{ &rootItem, {}, {}, true } };
	for (const auto& item : m_items)
	{
		chainItems.emplace_back(SDependencyChainItem{ &item, {&chainItems.front()}, {}, false });
		chainItems.front().children.push_back(&chainItems.back());
	}

	// Build dependency links
	for (const SDependencyLink& link : m_links)
	{
		SDependencyChainItem* pParent = nullptr;
		SDependencyChainItem* pChild = nullptr;
		for (SDependencyChainItem& chainItem : chainItems)
		{
			// Skip root node
			if (chainItem.pItem == nullptr)
				continue;

			const SGuid& itemSGuid = chainItem.pItem->guid;
			if (itemSGuid == link.parent)
				pParent = &chainItem;
			else if (itemSGuid == link.child)
				pChild = &chainItem;

			if (pParent != nullptr && pChild != nullptr)
				break;
		}

#ifdef _DEBUG
		assert((pParent && pChild) || (!pParent && !pChild));
#endif
		if (pParent != nullptr && pChild != nullptr)
		{
			pParent->children.push_back(pChild);
			pChild->parents.push_back(pParent);
		}
	}

#ifdef _DEBUG
	std::map<SDependencyChainItem*, bool> visited;
	for (SDependencyChainItem& chainItem : chainItems)
		if (chainItem.parents.size() == 1 && chainItem.parents.front()->pItem->name == "_root_")
			assert(!HaveCyclicDependency(&chainItem, visited));
#endif

	return chainItems;
}

// Depth First Search to check for cyclical dependencies
bool CDependencyResolver::HaveCyclicDependency(SDependencyChainItem* node, std::map<SDependencyChainItem*, bool>& visited) const
{
	std::stack<SDependencyChainItem*> stack;
	stack.push(node);

	while (!stack.empty())
	{
		node = stack.top();
		stack.pop();

		// Cyclical Dependency
		if (visited[node])
			return true;

		visited[node] = true;
		for (auto nextNode = node->children.begin(); nextNode != node->children.end(); nextNode++)
			if (!visited[*nextNode])
				stack.push(*nextNode);
	}

	return false;
}