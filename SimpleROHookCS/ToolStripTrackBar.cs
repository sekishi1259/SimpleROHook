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

        public TrackBar trackBar
        {
            get
            {
                return Control as TrackBar;
            }
        }

        // Add properties, events etc. you want to expose...
        public void SetMinMax(int min,int max)
        {
            trackBar.Minimum = min;
            trackBar.Maximum = max;
        }
        public void SetTickFrequency(int tick)
        {
            trackBar.TickFrequency = tick;
        }
        public int Value 
        {
            get {
                return trackBar.Value;
            }
            set {
                trackBar.Value = value;
            }
        }
        public void SetChangeValue(int small,int large)
        {
            trackBar.SmallChange = small;
            trackBar.LargeChange = large;
        }

        protected override void OnSubscribeControlEvents(Control c)
        {

        }
    }
}
