#ifndef MPE_SPARSE_SET_HPP
#define MPE_SPARSE_SET_HPP

#include <vector>
#include <utility>
#include <type_traits>
#include <cstdint>

namespace mpe
{
	template <typename T, uint32_t ELEMS_PER_PAGE_POT = 128>
	class SparseSet
	{
		static_assert(ELEMS_PER_PAGE_POT != 0);
		static_assert((ELEMS_PER_PAGE_POT & (ELEMS_PER_PAGE_POT - 1)) == 0);
	public:
		using id_t = uint32_t;
		T& Get(id_t key);
		const T& Get(id_t key) const;
		template <typename... Args>
		T& Set(id_t key, Args&&... args);
		const std::vector<id_t>& GetIds() const;
		const std::vector<T>& GetPackedElements() const;
	private:
		static constexpr id_t PAGE_MASK = static_cast<id_t>(ELEMS_PER_PAGE_POT - 1);
		using Page = std::vector<size_t>;
		//Sparse array divided in pages for a more efficient use of memory
		std::vector<Page> pages;
		//Packed array for identifiers
		std::vector<id_t> ids;
		//Packed array for actual data
		std::vector<T> packed;
	};

	template<typename T, uint32_t ELEMS_PER_PAGE_POT>
	T& SparseSet<T, ELEMS_PER_PAGE_POT>::Get(id_t key)
	{
		//Get Page
		const Page& page = pages[key / ELEMS_PER_PAGE_POT];
		//Get index
		size_t index = page[key & PAGE_MASK];
		//Get data
		return packed[index];
	}

	template<typename T, uint32_t ELEMS_PER_PAGE_POT>
	const T& SparseSet<T, ELEMS_PER_PAGE_POT>::Get(id_t key) const
	{
		//Get Page
		const Page& page = pages[key / ELEMS_PER_PAGE_POT];
		//Get index
		size_t index = page[key & PAGE_MASK];
		//Get data
		return packed[index];
	}

	template<typename T, uint32_t ELEMS_PER_PAGE_POT>
	const std::vector<uint32_t>& SparseSet<T, ELEMS_PER_PAGE_POT>::GetIds() const
	{
		return ids;
	}

	template<typename T, uint32_t ELEMS_PER_PAGE_POT>
	const std::vector<T>& SparseSet<T, ELEMS_PER_PAGE_POT>::GetPackedElements() const
	{
		return packed;
	}

	template<typename T, uint32_t ELEMS_PER_PAGE_POT>
	template<typename ...Args>
	T& SparseSet<T, ELEMS_PER_PAGE_POT>::Set(id_t key, Args && ...args)
	{
		//Get Page number
		size_t page_num = key / ELEMS_PER_PAGE_POT;

		//Generate as many Pages as needed
		if (pages.size() <= page_num)
			pages.resize(page_num + 1);

		//Get Page
		Page& page = pages[page_num];

		//Initialize the page if necessary
		if (page.empty())
			page.resize(ELEMS_PER_PAGE_POT);

		//Get index
		size_t masked_id = key & PAGE_MASK;
		size_t index = page[masked_id];

		size_t element_count = ids.size();
		//Check if the element is absent
		if ((index >= element_count) || (ids[index] != key))
		{
			//element_count is the index of the element
			page[masked_id] = element_count;
			ids.push_back(key);
			packed.emplace_back(std::forward<Args>(args)...);
		}
		else
			packed[index] = T(std::forward<Args>(args)...);

		return packed[index];
	}
}

#endif