using System.Windows.Forms;
using System.Windows.Forms.Design;


namespace SimpleROHookCS
{
    [ToolStripItemDesignerAvailability(ToolStripItemDesignerAvailability.MenuStrip |
                                   ToolStripItemDesignerAvailability.ContextMenuStrip)]
    public class ToolStripTrackBar : ToolStripControlHost
    {
        //private TrackBar trackBar;

        public ToolStripTrackBar() : base(new TrackBar()) { }

        public TrackBar TrackBarControl
        {
            get
            {
                return Control as TrackBar;
            }
        }

        // Add properties, events etc. you want to expose...
        public void SetMinMax(int min,int max)
        {
            TrackBarControl.Minimum = min;
            TrackBarControl.Maximum = max;
        }
        public void SetTickFrequency(int tick)
        {
            TrackBarControl.TickFrequency = tick;
        }
        public int Value 
        {
            get {
                return TrackBarControl.Value;
            }
            set {
                TrackBarControl.Value = value;
            }
        }
        public void SetChangeValue(int small,int large)
        {
            TrackBarControl.SmallChange = small;
            TrackBarControl.LargeChange = large;
        }

        protected override void OnSubscribeControlEvents(Control c)
        {
            // Call the base so the base events are connected.
            base.OnSubscribeControlEvents(c);
            // Cast Control to a TrackBar control.
            TrackBar trackBarContol = (TrackBar)c;
            // Add the event.
            trackBarContol.ValueChanged += 
                new System.EventHandler(OnValueChanged);
        }
        protected override void OnUnsubscribeControlEvents(Control c)
        {
            // Call the base method so the basic event are unsubscribed.
            base.OnUnsubscribeControlEvents(c);
            // Cast the controle to a TrackBar control.
            TrackBar trackBarContol = (TrackBar)c;
            // Remove the event.
            trackBarContol.ValueChanged -= 
                new System.EventHandler(OnValueChanged);
        }
        // Declare the TrackBar event.
        public event System.EventHandler ValueChanged;
        // Raise the TrackBar event.
        private void OnValueChanged(object sender,System.EventArgs e)
        {
            if( ValueChanged != null)
            {
                ValueChanged(this, e);
            }
        }
    }
}
