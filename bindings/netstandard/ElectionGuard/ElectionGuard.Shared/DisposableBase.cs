using System;
namespace ElectionGuard
{
    public class DisposableBase: IDisposable
    {
        private bool disposedValue;

        public DisposableBase()
        {
        }

        protected virtual void DisposeManaged()
        {
            // dispose managed state (managed objects)
        }

        protected virtual void DisposeUnamanged()
        {
            // free unmanaged resources (unmanaged objects) and override finalizer
            // set large fields to null
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    DisposeManaged();
                }
                DisposeUnamanged();
                disposedValue = true;
            }
        }

        // override finalizer only if 'Dispose(bool disposing)'
        // has code to free unmanaged resources
        ~DisposableBase()
        {
            // Do not change this code.
            // Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: false);
        }

        public void Dispose()
        {
            // Do not change this code.
            // Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
