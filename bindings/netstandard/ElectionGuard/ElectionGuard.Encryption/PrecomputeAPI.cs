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
        /// <param name="token">CancelationToken that can be used to start the process</param>
        /// <param name="buffers">The maximum number of buffers to precompute</param>
        void StartPrecomputeAsync(ElementModP publicKey, CancellationToken token, int buffers);

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
        private CancellationTokenSource cancelTokenSource;
        private CancellationToken cancelToken;
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
        /// Starts the precompute process by creating a new thread to run the process
        /// </summary>
        /// <param name="maxexp">The max exponentiation to be calculated</param>
        /// <param name="token">CancelationToken that can be used to start the process</param>
        [Obsolete]
        public void StartPrecomputeAsync(long maxexp, CancellationToken token)
        {
            cancelTokenSource = CancellationTokenSource.CreateLinkedTokenSource(cancelToken);
            cancelToken = token;
            currentStatus.CurrentState = PrecomputeState.Running;
        }

        /// <summary>
        /// Starts the precompute process by creating a new thread to run the process
        /// </summary>
        /// <param name="publicKey">The max exponentiation to be calculated</param>
        /// <param name="token">CancelationToken that can be used to start the process</param>
        /// <param name="buffers">The maximum number of buffers to precompute</param>
        public void StartPrecomputeAsync(ElementModP publicKey, CancellationToken token, int buffers = 0)
        {
            cancelTokenSource = CancellationTokenSource.CreateLinkedTokenSource(cancelToken);
            cancelToken = token;
            currentStatus.CurrentState = PrecomputeState.Running;
            max_buffers = buffers;
            elgamalPublicKey = publicKey;

            workerThread = new Thread(WorkerMethod);
            workerThread.Name = "Precompute Worker Thread";
            workerThread.Start();
        }

        /// <summary>
        /// Stops the precompute process
        /// </summary>
        public void StopPrecompute()
        {
            if (currentStatus.CurrentState == PrecomputeState.Running)
            {
                NativePrecomputeBuffers.Stop();
                currentStatus.CurrentState = PrecomputeState.UserStopped;
                cancelTokenSource?.Cancel();
            }
        }

        /// <summary>
        /// Dummy method to be used for the stubbing out until the exponentiation code is complete
        /// </summary>
        private void WorkerMethod()
        {
            if (cancelTokenSource == null)
            {
                cancelTokenSource = new CancellationTokenSource();
            }

            NativePrecomputeBuffers.Populate(elgamalPublicKey.Handle, max_buffers);

            if (cancelToken.IsCancellationRequested)
                currentStatus.CurrentState = PrecomputeState.UserStopped;
            else
                currentStatus.CurrentState = PrecomputeState.Completed;

            OnCompletedEvent();
        }

    }
}
