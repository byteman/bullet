#include "igraphicchannel.h"



IGraphicChannel::IGraphicChannel():
    m_filter(NULL)
{

}

IGraphicChannel::~IGraphicChannel()
{

}

void IGraphicChannel::SetFilter(IValueFilter *filter)
{
    m_filter=filter;
}
