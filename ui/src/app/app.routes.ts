import { Routes } from '@angular/router';
import { PageNotFoundComponent } from './pages/page-not-found.component';
import { DetailsComponent } from './pages/album/details.component';

export const routes: Routes = [
  {
    path: 'album',
    loadComponent: () =>
      import('./pages/album/album-home.component').then((m) => m.AlbumHomeComponent),
  },
  {
    path: 'chess',
    loadComponent: () =>
      import('./pages/chess/chess.component').then((m) => m.ChessComponent),
  },
  {
    path: 'details/:id',
    component: DetailsComponent,
    title: 'Album details',
  },

  { path: '', redirectTo: '/chess', pathMatch: 'full' },
  { path: '**', component: PageNotFoundComponent },
];
