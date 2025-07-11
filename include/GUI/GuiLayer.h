#ifndef GUILAYER_H
#define GUILAYER_H
#include "GUI/GuiElement.h"
#include "AnchorsType.h"
namespace gui {
  struct GuiEventContext {
    sf::Vector2i mousePos;
    bool f_hovering = false;
    bool f_clickDown = false;
    bool f_clickUp = false;
    bool f_mouseDown = false;

    void Reset() {
      mousePos = {};
      f_hovering = false;
      f_clickDown = false;
      f_clickUp = false;
      f_mouseDown = false;
    }
  };

  class GuiLayer : public Drawable{
  public:
    explicit GuiLayer(int reserveAmount = 3);
    explicit GuiLayer(Window* window, int reserveAmount = 3);
    ~GuiLayer() override {RemoveAllElements();}
    void AddElement(GuiElement* element);
    void RemoveElement(GuiElement* element);
    void RemoveAllElements();
    Vector2f GetPositionRelativeToAnchor(AnchorType anchor);

    void draw(RenderTarget& target, RenderStates states) const override;

    void callEvents();

    GuiEventContext ctx;
  private:
    std::vector<GuiElement*> elements;
    Window* window;
  };
}


#endif //GUILAYER_H
