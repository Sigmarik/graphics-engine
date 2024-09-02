#include "generic_meta.h"

GenericMeta::GenericMeta(const tinyxml2::XMLElement& element) {
    data_.InsertFirstChild(element.DeepClone(&data_));
}
