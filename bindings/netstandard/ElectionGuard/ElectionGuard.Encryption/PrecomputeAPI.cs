using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ElectionGuard
{
    using NativePrecomputeBuffers = NativeInterface.PrecomputeBuffers;


    /// <summary>
    /// States that the precompute process can be in.
    /// </summary>
    public enum PrecomputeState
    {
        /// <summary>
        /// The precompute has not been started.
        /// </summary>
        NotStarted = 0,
        /// <summary>
        /// The precompute is currently running
        /// </summary>
        Running = 1,
        /// <summary>
        /// The user stopped the precompute
        /// </summary>
        UserStopped = 2,
        /// <summary>
        /// The precompute finished
        /// </summary>
        Completed = 3,
    }

    /// <summary>
    /// Status for the precompute process
    /// </summary>
    public struct PrecomputeStatus
    {
        /// <summary>
        /// Percentage of the calculations that are complete
        /// </summary>
        public double Percentage;

        /// <summary>
        /// The number of exponentiations that are completed
        /// </summary>
        public long CompletedExponentiationsCount;

        /// <summary>
        /// Current status of the precompute process
        /// </summary>
        public PrecomputeState CurrentState;
    }

    /// <summary>
    /// Delegate definition to return back the status of the precompute process
    /// </summary>
    /// <returns><see cref="PrecomputeStatus">PrecomputeStatus</see> with all of the latest information</returns>
    public delegate void StatusEventHandler(PrecomputeStatus status);

    /// <summary>
    /// Interface for the precompute API to start and stop the precompute calculations
    /// </summary>
    public interface IPrecomputeAPI
    {
        /// <summary>
        /// Starts the precompute process by creating a new thread to run the process
        /// </summary>
        /// <param name="maxexp">The max exponentiation to be calculated</param>
        /// <param name="token">CancelationToken that can be used to start the process</param>
        [Obsolete]
        void StartPrecomputeAsync(long maxexp, CancellationToken token);

        /// <summary>
        /// Starts the precompute process by creating a new thread to run the process
        /// </summary>
        /// <param name="publicKey">The max exponentiation to be calculated</param>
        /// <param name="buffers">The maximum number of buffers to precompute</param>
        void StartPrecomputeAsync(ElementModP publicKey, int buffers);

        /// <summary>
        /// Stops the precompute process
        /// </summary>
        void StopPrecompute();

        /// <summary>
        /// Get the current status for the current running process
        /// </summary>
        /// <returns><see cref="PrecomputeStatus">PrecomputeStatus</see> with all of the latest information</returns>
        PrecomputeStatus GetStatus();

        /// <summary>
        /// Event handler that will give back progress to the calling code
        /// </summary>
        event StatusEventHandler ProgressEvent;

        /// <summary>
        /// Event handler that will signal that the entire precompute is completed
        /// </summary>
        event StatusEventHandler CompletedEvent;
    }

    /// <summary>
    /// Class that controls the precompute process
    /// </summary>
    public class Precompute : IPrecomputeAPI
    {
        AutoResetEvent waitHandle;

        private PrecomputeStatus currentStatus = new PrecomputeStatus
        {
            Percentage = 0,
            CompletedExponentiationsCount = 0,
            CurrentState = PrecomputeState.NotStarted
        };
        private Thread workerThread;
        private int max_buffers = 0;    // zero will say to run to a default number of runs
        private ElementModP elgamalPublicKey;

        /// <summary>
        /// Event handler that will give back progress to the calling code
        /// </summary>
        public event StatusEventHandler ProgressEvent;

        /// <summary>
        /// Event handler that will signal that the entire precompute is completed
        /// </summary>
        public event StatusEventHandler CompletedEvent;

        /// <summary>
        /// Internal method used to call the completed event
        /// </summary>
        private void OnCompletedEvent()
        {
            CompletedEvent?.Invoke(currentStatus);
        }

        /// <summary>
        /// Internal method used to call the progress event
        /// </summary>
        private void OnProgressEvent()
        {
            ProgressEvent?.Invoke(currentStatus);
        }

        /// <summary>
        /// Get the current status for the current running process
        /// </summary>
        /// <returns><see cref="PrecomputeStatus">PrecomputeStatus</see> with all of the latest information</returns>
        public PrecomputeStatus GetStatus()
        {
            return currentStatus;
        }

        /// <summary>
        /// Gets the progress of the precompute
        /// </summary>
        /// <param name="count">count of the buffer entries</param>
        /// <param name="queue_size">max size of the buffer queue</param>
        public void GetProgress(out int count, out int queue_size)
        {
            NativePrecomputeBuffers.Status(out count, out queue_size);
        }

        /// <summary>
        /// Starts the precompute process by creating a new thread to run the process
        /// </summary>
        /// <param name="maxexp">The max exponentiation to be calculated</param>
        /// <param name="token">CancelationToken that can be used to start the process</param>
        [Obsolete]
        public void StartPrecomputeAsync(long maxexp, CancellationToken token)
        {
            currentStatus.CurrentState = PrecomputeState.Running;
        }

        /// <summary>
        /// Starts the precompute process by creating a new thread to run the process
        /// </summary>
        /// <param name="publicKey">The max exponentiation to be calculated</param>
        /// <param name="buffers">The maximum number of buffers to precompute</param>
        public void StartPrecomputeAsync(ElementModP publicKey, int buffers = 0)
        {
            currentStatus.CurrentState = PrecomputeState.Running;
            max_buffers = buffers;
            elgamalPublicKey = publicKey;

            waitHandle = new AutoResetEvent(false);
            workerThread = new Thread(WorkerMethod);
            workerThread.Name = "Precompute Worker Thread";
            workerThread.Start();

            waitHandle.WaitOne();   // make sure thread is created before returning
        }

        /// <summary>
        /// Stops the precompute process
        /// </summary>
        public void StopPrecompute()
        {
            if (currentStatus.CurrentState == PrecomputeState.Running)
            {
                currentStatus.CurrentState = PrecomputeState.UserStopped;
            }
            else
            {
                // if this is already stopped or completed, resend the Completed event to make sure calling app sees the stopping
                SendCompleted();
            }
            NativePrecomputeBuffers.Stop();     // tell the calculations to 
        }

        /// <summary>
        /// Dummy method to be used for the stubbing out until the exponentiation code is complete
        /// </summary>
        private void WorkerMethod()
        {
            NativePrecomputeBuffers.Init(max_buffers);
            waitHandle.Set();
            NativePrecomputeBuffers.Populate(elgamalPublicKey.Handle);

            SendCompleted();
        }

        private void SendCompleted()
        {
            int count = -1;
            int queue_size = -2;
            GetProgress(out count, out queue_size);
            if (count == queue_size)
                currentStatus.CurrentState = PrecomputeState.Completed;
            currentStatus.Percentage = (double)count / queue_size;
            currentStatus.CompletedExponentiationsCount = count;

            OnCompletedEvent();
        }

    }
}
