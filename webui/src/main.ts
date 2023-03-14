import { createApp } from 'vue'
import Play from './Play.vue'


import './assets/main.css'

declare module 'vue' {
  interface ComponentCustomProperties {
    $play_url: string
  }
}

const app = createApp(Play);
app.config.globalProperties.$play_url = "";
app.mount('#app')
