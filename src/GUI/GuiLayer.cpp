#include "GUI/GuiLayer.h"
#include "engine/Application.h"
namespace gui {
    GuiLayer::GuiLayer(int reserveAmount) {
        elements.reserve(reserveAmount);
        this->window = Application::getInstance()->getWindow();
    }

    GuiLayer::GuiLayer(Window* window, int reserveAmount) {
        elements.reserve(reserveAmount);
        this->window = window;
    }

    void GuiLayer::AddElement(GuiElement* element) {
        elements.push_back(element);
        element->setParent(this);
        element->SetRelativePosition(element->GetRelativePosition());
        element->NormalizePositionRelativeToParent(element->GetNormalizedScale());
    }

    void GuiLayer::RemoveElement(GuiElement* element) {
        for (auto it = elements.begin(); it != elements.end(); it++) {
            if (*it == element) {
                elements.erase(it);
                delete element;
            }
        }
    }

    void GuiLayer::RemoveAllElements() {
        for (auto element : elements) {
            delete element;
        }
        elements.clear();
    }


    Vector2f GuiLayer::GetPositionRelativeToAnchor(AnchorType anchor) {
        float width = static_cast<float>(window->getSize().x);
        float height = static_cast<float>(window->getSize().x);
        switch (anchor) {
            case TopLeft: return {0, 0};
            case TopRight: return {width,0};
            case TopCenter: return {width/2.0f, 0};
            case MiddleLeft: return {0,height/2.0f};
            case MiddleCenter: return {width/2.0f, height/2.0f};
            case MiddleRight: return {width,height/2.0f};
            case BottomLeft: return {0,height};
            case BottomCenter: return {width/2.0f,height};
            case BottomRight: return {width,height};
        }
    }

    void GuiLayer::draw(RenderTarget &target, RenderStates states) const {
        for (const auto element : elements) {
            target.draw(*element, states);
        }
    }

    void callEventsInternal(GuiElement* element, GuiEventContext& ctx, bool& first) {
        if (element->isHidden()) return;
        GuiElementEventContext elementCtx{};
        elementCtx.mousePos = ctx.mousePos;

        if (element->isInsideBoundingBox(ctx.mousePos) && first) {
            elementCtx.f_hovering = true;
            elementCtx.f_clickDown = ctx.f_clickDown;
            elementCtx.f_mouseDown = ctx.f_mouseDown;
            first = false;
        }

        element->ctx = elementCtx;
        element->update();
    }

    void GuiLayer::callEvents() {
        bool first = true;
        for (const auto element : std::vector<GuiElement*>(elements.rbegin(), elements.rend())) {
            for (const auto child : std::vector<GuiElement*>(element->getChildren()->rbegin(), element->getChildren()->rend())) {
                callEventsInternal(child, ctx, first);
            }
            callEventsInternal(element, ctx, first);
        }
        ctx.Reset();
    }
}
