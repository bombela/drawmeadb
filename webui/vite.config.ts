import { fileURLToPath, URL } from 'node:url'

import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

// https://vitejs.dev/config/
export default defineConfig(({command, mode}) => {
  let play_url: string = process.env.PLAY_URL || '/play';
  if (command === 'serve') {
    play_url = '"' + play_url + '"';
  }
  return {
    plugins: [vue()],
    resolve: {
      alias: {
        '@': fileURLToPath(new URL('./src', import.meta.url))
      }
    },
    base: '/play',
    define: {
      __PLAY_URL__: play_url,
    },
  }
})
