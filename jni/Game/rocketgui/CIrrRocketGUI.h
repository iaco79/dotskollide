#ifndef CIRRROCKETGUI_H
#define CIRRROCKETGUI_H

namespace irr
{
    class IrrlichtDevice;
    class IEventReceiver;
    namespace scene
    {
        class ICameraSceneNode;
    }
}
namespace Rocket
{
    namespace Core
    {
        class Context;
    }
}
class CIrrRocketEventReceiver;
class CIrrRocketGUI
{
    public:
        /** Default constructor */
        CIrrRocketGUI(irr::IrrlichtDevice* device);
        /** Default destructor */
        virtual ~CIrrRocketGUI();
		void Initialize();

        irr::IEventReceiver* getGUIEventReceiver(void);


		bool LoadDocument(const char* documentPath,bool show=true);
		bool CloseDocument(const char* documentId);		
		bool SetDocumentVisible(const char* documentId, const bool visible);

		bool SetElementInnerText(const char* documentId,const char* elementName, const char* value);
		bool SetElementInnerText(const char* documentId,const char* elementName, const int value);
		bool SetElementInnerText(const char* documentId,const char* elementName, const float value);
		bool SetElementVisibility(const char* documentId,const char* elementName, const bool visible);
		
		void SetSkipGUIEvents(bool skipEvents) {  mSkipGUIEvents = skipEvents; } 
		bool GetSkipGUIEvents() {  return mSkipGUIEvents; } 

		void Reset();

        void run(void);
    protected:
        irr::IrrlichtDevice* Device;
        Rocket::Core::Context* RocketContext;

        irr::scene::ICameraSceneNode* GUI_Camera;
        CIrrRocketEventReceiver* InputEvents;

		IrrRocketFileInterface* mRocketFS;
		IrrRocketRenderer* mRocketRenderer;
		IrrRocketInterface* mRocketInterface;


        static unsigned int RocketContextCount;
		bool mSkipGUIEvents;
    private:
};

#endif // CIRRROCKETGUI_H
