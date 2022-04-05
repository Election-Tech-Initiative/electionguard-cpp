using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ElectionGuard
{
    /// <summary>
    /// States that the precompute process can be in.
    /// </summary>
    public enum PrecomputeState
    {
        /// <summary>
        /// The precompute has not been started.
        /// </summary>
        NotStarted,
        /// <summary>
        /// The precompute is currently running
        /// </summary>
        Running,
        /// <summary>
        /// The user stopped the precompute
        /// </summary>
        UserStopped,
        /// <summary>
        /// The precompute finished
        /// </summary>
        Completed,
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
        /// <returns></returns>
        void StartPrecomputeAsync(long maxexp, CancellationToken token);

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
        private long delay = 2;

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
        public void StartPrecomputeAsync(long maxexp, CancellationToken token)
        {
            cancelTokenSource = CancellationTokenSource.CreateLinkedTokenSource(cancelToken);
            cancelToken = token;
            currentStatus.CurrentState = PrecomputeState.Running;
            delay = maxexp;
            workerThread = new Thread(workerMethod);
            workerThread.Start();
        }

        /// <summary>
        /// Stops the precompute process
        /// </summary>
        public void StopPrecompute()
        {
            if (currentStatus.CurrentState == PrecomputeState.Running)
            {
                currentStatus.CurrentState = PrecomputeState.UserStopped;
                cancelTokenSource?.Cancel();
            }
        }

        private void workerMethod()
        {
            double progress = 0;
            if (cancelTokenSource == null)
            {
                cancelTokenSource = new CancellationTokenSource();
            }
            while (!cancelToken.IsCancellationRequested)
            {
                Thread.Sleep((int)delay * 1000);

                if (cancelToken.IsCancellationRequested)
                {
                    currentStatus.CurrentState = PrecomputeState.UserStopped;
                    OnCompletedEvent();
                    return;
                }

                progress += 10;
                currentStatus.Percentage = progress;
                if (progress == 100)
                {
                    currentStatus.CurrentState = PrecomputeState.Completed;
                    OnCompletedEvent();
                    return;
                }
                else
                {
                    OnProgressEvent();
                }
            }
        }

    }
}
