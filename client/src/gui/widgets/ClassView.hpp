#pragma once
#include "Widget.hpp"

namespace hot_spotter::gui::widget {
    class ClassView final : public Widget {
    public:
        explicit ClassView() = default;

        void Init() override;

        void Render() override;
    };
} // namespace hot_spotter::gui::widget
