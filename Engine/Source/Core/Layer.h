#ifndef ENGINE_LAYER_H
#define ENGINE_LAYER_H

namespace Engine
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float ts) {}

        virtual void OnImGuiRender() {}

        const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
} // namespace Engine

#endif // ENGINE_LAYER_H
