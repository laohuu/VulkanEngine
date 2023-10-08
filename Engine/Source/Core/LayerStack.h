#ifndef ENGINE_LAYERSTACK_H
#define ENGINE_LAYERSTACK_H

#include "Layer.h"

namespace Engine
{
    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        Layer* operator[](size_t index) { return m_Layers[index]; }

        const Layer* operator[](size_t index) const { return m_Layers[index]; }

        size_t Size() const { return m_Layers.size(); }

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int        m_LayerInsertIndex = 0;
    };
} // namespace Engine

#endif // ENGINE_LAYERSTACK_H
