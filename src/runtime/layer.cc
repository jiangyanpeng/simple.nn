#include "runtime/layer.h"

#include <log.h>

namespace nn {

MStatus Layer::Init(const std::shared_ptr<LayerParam>& param,
                    const std::shared_ptr<ModelBin>& bin) {
    SIMPLE_LOG_DEBUG("{} Layer::Load Start", name_);
    SIMPLE_LOG_DEBUG("{} Layer::Load End", name_);
    return MStatus::M_OK;
}

MStatus Layer::Forward(const std::vector<TensorPtr>& input, std::vector<TensorPtr>& output) {
    SIMPLE_LOG_DEBUG("{} Layer::Load Start", name_);
    SIMPLE_LOG_DEBUG("{} Layer::Load End", name_);
    return MStatus::M_NOT_SUPPORT;
}

} // namespace nn
