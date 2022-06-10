#ifndef MPE_DATA_ORIENTED_HPP
#define MPE_DATA_ORIENTED_HPP

#include <MPE/DataStructures/SparseSet.hpp>
#include <cstdint>

//====================DO NOT USE THESE============================

//Don't use this
#define MPE_DOD_ID_TYPE(DataType) DataType##Id

//======================DECLARATIONS==============================

//Declare the ID type of the specified data based on uint32_t
#define MPE_DOD_DECLARE_ID_TYPE(DataType) \
struct MPE_DOD_ID_TYPE(DataType) \
{ \
	uint32_t value; \
	constexpr MPE_DOD_ID_TYPE(DataType)() : value(0u) { }	\
	MPE_DOD_ID_TYPE(DataType)(uint32_t value_) : value(value_) { } \
	bool operator==(MPE_DOD_ID_TYPE(DataType) other) const { return value == other.value; } \
	bool operator!=(MPE_DOD_ID_TYPE(DataType) other) const { return value != other.value; } \
};

//Declares the Read function to read data
#define MPE_DOD_DECLARE_READ(DataType) \
const DataType& Read##DataType(MPE_DOD_ID_TYPE(DataType) id); \
const std::vector<DataType>& Read##DataType##Buffer(); \
const std::vector<uint32_t>& Read##DataType##Ids();


//Declares the getter and setter functions to write data
#define MPE_DOD_DECLARE_WRITE(DataType) \
DataType& Get##DataType(MPE_DOD_ID_TYPE(DataType) id); \
void Set##DataType(MPE_DOD_ID_TYPE(DataType) id, const DataType& data);

//=====================IMPLEMENTATIONS============================

//Implements declarations (goes in a cpp file)
#define MPE_DOD_IMPLEMENT(DataType) \
static mpe::SparseSet<DataType> buffer;	\
const DataType& Read##DataType(MPE_DOD_ID_TYPE(DataType) id) \
{ \
	return buffer.Get(id.value); \
} \
const std::vector<DataType>& Read##DataType##Buffer() \
{ \
	return buffer.GetPackedElements(); \
} \
const std::vector<uint32_t>& Read##DataType##Ids() \
{ \
	return buffer.GetIds(); \
} \
DataType& Get##DataType(MPE_DOD_ID_TYPE(DataType) id) \
{ \
	return buffer.Get(id.value); \
} \
void Set##DataType(MPE_DOD_ID_TYPE(DataType) id, const DataType& data) \
{ \
	buffer.Set(id.value, data); \
}


#endif