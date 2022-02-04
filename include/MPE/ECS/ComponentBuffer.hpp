#ifndef MPE_COMPONENT_BUFFER_HPP
#define MPE_COMPONENT_BUFFER_HPP

#include <utility>
#include <vector>
#include <MPE/Core/Logger.hpp>
#include <MPE/Core/Utils.hpp>
#include "Entity.hpp"

namespace mpe
{
	class IComponentBuffer
	{
	public:
		virtual ~IComponentBuffer() = default;
	};

	template <typename C>
	class ComponentBuffer : public IComponentBuffer
	{
	public:
		ComponentBuffer() = default;
		//Returns a collection of Entities associated with the Component
		const std::vector<Entity>& Entities() const;
		//Creates a Component for the specified Entity if it doesn't exist and writes to it
		template <typename... Args>
		C& SetComponent(Entity e, Args&&... args);
		//Returns the Component associated to the specified Entity
		C& GetComponent(Entity e);
	private:
		//Paging lets us store indices in sparse arrays without wasting too much memory
		using Page = std::vector<size_t>;
		//Feel free to change it as long as it's a power of two
		static constexpr size_t ENTITIES_PER_PAGE = 256;
		//Every Page is a sparse array to index the Entities (and Components, ordered accordingly)
		std::vector<Page> pages;
		//Packed array for Entities
		std::vector<Entity> entities;
		//Packed array for Components
		std::vector<C> data;
	};

	template<typename C>
	const std::vector<Entity>& ComponentBuffer<C>::Entities() const
	{
		return entities;
	}

	template<typename C>
	C& ComponentBuffer<C>::GetComponent(Entity e)
	{
		//Calculate correct Page
		size_t page_index = e / ENTITIES_PER_PAGE;
		const Page& indexer = pages[page_index];
		//Calculate correct index
		size_t index = indexer[e & (ENTITIES_PER_PAGE - 1)];

		return data[index];
	}

	template<typename C>
	template<typename... Args>
	C& ComponentBuffer<C>::SetComponent(Entity e, Args&&... args)
	{
		//Calculate correct Page
		size_t page_index = e / ENTITIES_PER_PAGE;

		//Generate the Page if necessary
		if (page_index >= pages.size())
			pages.resize(page_index + 1);
		Page& page = pages[page_index];

		//Initialize the sparse set if necessary
		if (page.empty())
			page.resize(ENTITIES_PER_PAGE);

		//Get the actual index in the Page
		page_index = e & (ENTITIES_PER_PAGE - 1);
		size_t packed_index = page[page_index];

		//Check if the Entity is already associated with a Component
		if (entities.empty() || entities[packed_index] != e)
		{
			//Insert the index of the next entity in the calculated Page
			page[page_index] = entities.size();
			//Add the entity
			entities.push_back(e);
			data.emplace_back(std::forward<Args>(args)...);
		}
		else
			data.emplace(data.begin() + packed_index, std::forward<Args>(args)...);

		return data[packed_index];
	}

}

#endif