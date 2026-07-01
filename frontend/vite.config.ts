import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// Dev server config. The proxy is the important bit: during `npm run dev`,
// requests the app makes to "/api/*" are forwarded to the C++ backend on
// :8080. That means the browser talks to one origin (the Vite server) and
// never hits a CORS wall in development.
export default defineConfig({
  plugins: [react()],
  server: {
    port: 5173,
    proxy: {
      "/api": {
        target: "http://localhost:8080",
        changeOrigin: true,
      },
    },
  },
});
